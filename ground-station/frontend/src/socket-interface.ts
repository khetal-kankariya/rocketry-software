import { toast } from 'svelte-sonner';
import {
	activeBaudRate,
	activePort,
	initTime,
	last_command,
	missionProgress,
	readData,
	sensors_calibrated,
	server_connected,
	storecsvdata,
	telemetry_running,
	travelled_coords
} from './store';
import { io } from 'socket.io-client';
import type { LatLngExpression } from 'leaflet';

var socket = io('http://localhost:5000', {
	autoConnect: false
});

export const server_exists = () => {
	return;
};

export const connect_socket = () => {
	socket.connect();
	window.sessionStorage.setItem('server-connected', 'true');
	server_connected.set(true);
};

export const disconnect_socket = () => {
	socket.disconnect();
	window.sessionStorage.setItem('server-connected', 'false');
	server_connected.set(false);
};

const socket_status_toaster = () => {
	socket.on('connect', () => {
		server_connected_toast();
		get_telemetry_status();
		get_calibration_status();
	});
	socket.on('connect_error', () => {
		disconnect_socket();
		server_disconnected_toast();
	});
	socket.on('disconnect', () => {
		server_disconnected_toast();
	});
};
socket_status_toaster();

const server_connected_toast = () => {
	toast.success('Server Connected!', {
		duration: 1500
	});
};
const server_disconnected_toast = () => {
	toast.error('Server disconnected!', {
		duration: 1500
	});
};

const get_telemetry_status = () => {
	socket.emit('get-telemetry-state', (res: boolean) => {
		telemetry_running.set(res);
	});
};
get_telemetry_status();

const get_calibration_status = () => {
	socket.emit('get-calibration-state', (res: boolean) => {
		sensors_calibrated.set(res);
	});
};
get_calibration_status();

let startupdata: Tstartupdata;

// setting start-up data to appropriate stores
export const get_startupdata = () => {
	if (!window.sessionStorage.getItem('startupdata')) {
		socket.emit(
			'get-startupdata',
			(data: {
				startupdata: {
					startTime: number;
					selectedPort: string;
					baudRateList: number;
				};
			}) => {
				window.sessionStorage.setItem('startupdata', JSON.stringify(data.startupdata));
				save_startupdata();
			}
		);
	}
};

export const save_startupdata = () => {
	startupdata = JSON.parse(window.sessionStorage.getItem('startupdata') || '{}');
	if (Object.keys(startupdata).length > 0) {
		let { startTime, selectedPort, selectedBaudRate } = startupdata;
		activePort.set(selectedPort);
		activeBaudRate.set(selectedBaudRate);
		initTime.set(startTime);
	}
};

let velocity_vals: number[] = [
	5.35, 5.39, 5.13, 5.04, 5.26, 5.14, 5.12, 5.07, 4.8, 4.84, 4.99, 4.54, 4.58, 4.81, 4.78, 4.61,
	4.64, 4.63, 4.77, 4.45, 4.54, 4.26, 4.42, 4.23, 4.24, 4.28, 4.42, 4.31, 4.44, 4.36, 4.36, 4.27,
	4.29, 4.41, 4.33, 4.24, 4.22, 4.2, 4.44, 4.36, 4.27, 4.0, 4.42, 4.33, 4.1, 4.18, 4.14, 4.11, 4.21,
	4.08, 4.24, 4.16
];
// listing to socket'datasent' and setting to store variable
const get_data = () => {
	socket.on('datasent', (newData: TtelemetryData) => {
		readData.update((prevData) => {
			if (prevData?.packetCount == newData?.packetCount) {
				return prevData;
			} else {
				newData.velocity = velocity_vals[newData.packetCount % velocity_vals.length];
				return newData;
			}
		});
		add_coords([newData.gnss_latitude, newData.gnss_longitude]);
	});
};
get_data();

const add_coords = (coord: LatLngExpression) => {
	travelled_coords.update((val) => {
		return [...val, coord];
	});
};

// listing to socket'csv-data-received' and setting to store variable
export const get_csv_data = () => {
	socket.emit('get-csv-data', (csvdata: { data: string[] }) => {
		let scsvdata = csvdata.data;

		storecsvdata.set(scsvdata);
	});
};

export const toggle_telemetry = (pres_telemetry_running: boolean) => {
	let response: boolean = false;
	if (socket.connected == true) {
		socket.emit('toggle-telemetry', pres_telemetry_running, (res: boolean) => {
			telemetry_running.set(res);
			response = res;
			if (res) {
				last_command.set('telestart');
				window.sessionStorage.setItem('last-command', 'telestart');
				toast.success('Telemetry Started', {
					duration: 1500
				});
			} else {
				last_command.set('telestop');
				window.sessionStorage.setItem('last-command', 'telestop');
				toast.success('Telemetry Stopped', {
					duration: 1500
				});
			}
		});
	} else {
		server_disconnected_toast();
	}
	return response;
};

export const calibrate_sensors = async () => {
	let response = false;
	if (socket.connected) {
		last_command.set('calibrate');
		window.sessionStorage.setItem('last-command', 'calibrate');
		const calibration_status: { status: string; message: string } = await new Promise((resolve) => {
			socket.emit('calibrate-sensors', (res: { status: string; message: string }) => {
				resolve(res);
			});
		});
		if (calibration_status.status == 'success') {
			sensors_calibrated.set(true);

			toast.success('Sensor Calibrated', {
				duration: 1500
			});

			response = true;
		} else {
			sensors_calibrated.set(false);

			toast.error('Sensor Calibration Failed', {
				duration: 1500
			});

			console.log(calibration_status);
			response = false;
		}
	} else {
		server_disconnected_toast();
	}

	return response;
};

export const get_mission_progress = () => {
	socket.emit('get-mission-progress', (res: number) => {
		missionProgress.set(res * 100);
	});
};
