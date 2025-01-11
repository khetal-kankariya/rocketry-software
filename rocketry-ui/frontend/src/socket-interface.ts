import { toast } from 'svelte-sonner';
import {
	activeBaudRate,
	activePort,
	initTime,
	missionProgress,
	readData,
	sensor_calibrated,
	storecsvdata,
	telemetry_running,
	travelled_coords
} from './store';
import { io } from 'socket.io-client';
import type { LatLngExpression } from 'leaflet';

var socket = io('http://localhost:5000');

export const socket_connected = () => {
	return socket.connected;
};

const connect_socket = () => {
	if (!socket_connected()) {
		setTimeout(() => {
			socket.connect;
			connect_socket();
		}, 1000);
	}
};
connect_socket();

const socket_status_toaster = () => {
	socket.on('connect', () => {
		toast.success('Server Connected!');
	});
	socket.on('connect_error', () => {
		server_disconnected_toast();
		get_telemetry_status();
	});
};
socket_status_toaster();

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

// listing to socket'datasent' and setting to store variable
const get_data = () => {
	socket.on('datasent', (newData: TtelemetryData) => {
		readData.update((prevData) => {
			if (prevData?.packetCount == newData?.packetCount) {
				return prevData;
			} else {
				return newData;
			}
		});
		add_coords([newData.gnss.lat, newData.gnss.lng]);
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
	if (socket_connected() == true) {
		socket.emit('toggle-telemetry', pres_telemetry_running, (res: boolean) => {
			telemetry_running.set(res);
			response = res;
			if (res) {
				toast.success('Telemetry Started', {
					duration: 1500
				});
			} else {
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
	if (socket_connected()) {
		let calibration_status: { status: string; message: string } = await new Promise((resolve) => {
			socket.emit('calibrate-sensors', (res: { status: string; message: string }) => {
				resolve(res);
			});
		});
		if (calibration_status.status == 'success') {
			sensor_calibrated.set(true);

			toast.success('Sensor Calibrated', {
				duration: 1500
			});

			response = true;
		} else {
			sensor_calibrated.set(false);

			toast.error('Sensor Calibration Failed', {
				duration: 1500
			});

			response = false;
		}
	}

	return response;
};

export const get_mission_progress = () => {
	socket.emit('get-mission-progress', (res: number) => {
		missionProgress.set(res * 100);
	});
};
