import { toast } from 'svelte-sonner';
import {
	last_command,
	missionProgress,
	payloadData,
	sensors_calibrated,
	server_connected,
	server_connecting,
	storecsvdata,
	telemetry_running
} from '@src/store';
import { io } from 'socket.io-client';
import type { LatLngExpression } from 'leaflet';

let server_connected_local: boolean;
server_connected.subscribe((val) => {
	server_connected_local = val;
});

const socket = io('http://localhost:5000', {
	autoConnect: false
});

export const socket_init = () => {
	console.log(socket);
};

export const connect_socket = () => {
	socket.connect();

	let i = 0;
	while (i++ < 500 && !socket.connected) server_connecting.set(true);
};

export const disconnect_socket = () => {
	socket.disconnect();

	let i = 0;
	while (i++ < 500 && !socket.disconnected) server_connecting.set(true);
};

/**
 * Sets up event listeners for the socket connection to handle connection status changes.
 *
 * - Listens for the `connect` event to invoke the `server_connected_fn` callback when the server is connected.
 * - Listens for the `connect_error` event to invoke the `server_disconnected_fn` callback when there is a connection error.
 * - Listens for the `disconnect` event to invoke the `server_disconnected_fn` callback when the server is disconnected.
 */
const socket_status_fn = () => {
	socket.on('connect', () => {
		server_connected_fn();
	});
	socket.on('connect_error', () => {
		server_disconnected_fn();
	});
	socket.on('disconnect', () => {
		server_disconnected_fn();
	});
};
socket_status_fn();

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

/**
 * Handles the actions to be performed when the server connection is successfully established.
 *
 * This function performs the following tasks:
 * - Updates the `server_connected` and `server_connecting` states to reflect the connection status.
 * - Fetches the telemetry and calibration statuses from the server.
 * - Stores the server connection status in the session storage for persistence.
 * - Displays a toast notification to inform the user about the successful connection.
 */
const server_connected_fn = () => {
	server_connected.set(true);
	server_connecting.set(false);

	get_telemetry_status();
	get_calibration_status();

	window.sessionStorage.setItem('server-connected', 'true');

	server_connected_toast();
};

/**
 * Handles the actions to be performed when the server is disconnected.
 *
 * - Sets `server_connected` and `server_connecting` to `false` to reflect  the disconnection state.
 * - Updates the `server-connected` key in `window.sessionStorage` to `'false'` for persistence across page reloads.
 * - Invokes the `server_disconnected_toast` function to notify the user about the disconnection.
 */
const server_disconnected_fn = () => {
	server_connected.set(false);
	server_connecting.set(false);

	window.sessionStorage.setItem('server-connected', 'false');

	server_disconnected_toast();
};

const get_telemetry_status = () => {
	socket.emit('get-telemetry-state', (res: boolean) => {
		telemetry_running.set(res);
	});
};

const get_calibration_status = () => {
	socket.emit('get-calibration-state', (res: boolean) => {
		sensors_calibrated.set(res);
	});
};

let startupdata: Tstartupdata;

export const get_startupdata = () => {
	if (!window.sessionStorage.getItem('startupdata')) {
		socket.emit(
			'get-startupdata',
			(data: {
				startupdata: {
					startTime: number;
				};
			}) => {
				window.sessionStorage.setItem('startupdata', JSON.stringify(data.startupdata));
			}
		);
	}
};

/**
 * Sets up a socket listener for the 'datasent' event to handle incoming telemetry data.
 *
 * When new telemetry data is received, it updates the `readData` store with the new data if the packet count differs from the previous data. Additionally, it adds the GNSS
 * latitude and longitude coordinates to the coordinate list.
 *
 * @remarks
 * - The `datasent` event is expected to emit data of type `TtelemetryData`.
 * - The `readData` store is updated only if the `packetCount` of the new data differs
 *   from the previous data.
 * - GNSS coordinates are extracted from the `gnss_latitude` and `gnss_longitude` fields
 *   of the telemetry data.
 */
const get_data = () => {
	socket.on('datasent', (newData: TpayloadData) => {
		payloadData.update((prevData) => {
			if (prevData.packetCount == newData.packetCount) {
				return prevData;
			} else {
				return newData;
			}
		});
	});
};
get_data();

/**
 * Fetches CSV data from the server via a socket event and updates the store with the received data.
 *
 * This function emits a `get-csv-data` event to the server using a socket connection.
 * The server responds with an object containing a `data` property, which is an array of strings
 * representing the CSV data. The received data is then stored in a reactive store (`storecsvdata`).
 */
export const get_csv_data = () => {
	socket.emit('get-csv-data', (csvdata: { data: string[] }) => {
		const scsvdata = csvdata.data;

		storecsvdata.set(scsvdata);
	});
};

const update_last_command = (command_string: Tcommand_from_gcs) => {
	last_command.set(command_string);
	window.sessionStorage.setItem('last-command', command_string || '');
};

const command_toast_success = (command_string: Tcommand_from_gcs) => {
	const toast_text =
		command_string == 'telestart'
			? 'Telemetry Started'
			: command_string == 'telestop'
				? 'Telemetry Stopped'
				: command_string == 'calibrate'
					? 'Sensors Calibrated'
					: '';
	toast.success(toast_text, {
		duration: 1500
	});
};

const command_toast_failure = (command_string: Tcommand_from_gcs) => {
	const toast_text =
		command_string == 'telestart'
			? 'Could not start Telemetry '
			: command_string == 'telestop'
				? 'Could not stop Telemetry'
				: command_string == 'calibrate'
					? 'Could not calibrate Sensors'
					: '';
	toast.error(toast_text, {
		duration: 1500
	});
};

export const toggle_telemetry = (pres_telemetry_running: boolean) => {
	let response: {
		status: string;
		telemetry_running: boolean;
		message: string;
	} = {
		status: 'yet to send',
		telemetry_running: false,
		message: 'none'
	};
	socket.emit('toggle-telemetry', pres_telemetry_running, (res: typeof response) => {
		telemetry_running.set(res.telemetry_running);
		response = res;
		if (res) {
			update_last_command('telestart');
		} else {
			update_last_command('telestop');
		}
	});

	telemetry_running.set(response.telemetry_running);
};

export const calibrate_sensors = async () => {
	let response = false;
	if (server_connected_local) {
		update_last_command('calibrate');
		const calibration_status: { status: string; message: string } = await new Promise((resolve) => {
			socket.emit('calibrate-sensors', (res: { status: string; message: string }) => {
				resolve(res);
			});
		});
		if (calibration_status.status == 'success') {
			sensors_calibrated.set(true);

			command_toast_success('calibrate');

			response = true;
		} else {
			sensors_calibrated.set(false);

			command_toast_failure('calibrate');

			response = false;
		}
	} else {
		server_disconnected_fn();
	}

	return response;
};

export const get_mission_progress = () => {
	socket.emit('get-mission-progress', (res: number) => {
		missionProgress.set(res * 100);
	});
};
