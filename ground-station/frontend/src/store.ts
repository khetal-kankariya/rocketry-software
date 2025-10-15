import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const GCStype: Writable<null | 'rocket' | 'payload'> = writable(null);

export const apiconnected = writable(false);

export const rocketAllDataNames: Readable<TrocketField[]> = readable([
	'team_id',
	'time_stamp',
	'packet_count',
	'altitude',
	'pressure',
	'temperature',
	'voltage',
	'gnss_time',
	'gnss_latitude',
	'gnss_longitude',
	'gnss_altitude',
	'gnss_sats',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'software_state',
	'gas_concentration',
	'velocity'
]);

export const payloadAllDataNames: Readable<TrocketField[]> = readable([
	'team_id',
	'packet_count',
	'altitude',
	'pressure',
	'temperature',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'software_state',
	'gas_concentration',
	'velocity'
]);

export const rocketDataInfo: Readable<{
	[T in TrocketField]: {
		graph: boolean;
		unit: string;
		displayName: string;
	};
}> = readable({
	team_id: { graph: false, unit: '', displayName: 'Team ID' },
	time_stamp: {
		graph: false,
		unit: '',
		displayName: 'Time Stamp'
	},
	packet_count: {
		graph: false,
		unit: '',
		displayName: 'Packet Count'
	},
	altitude: { graph: true, unit: 'm', displayName: 'Altitude' },
	pressure: { graph: true, unit: 'Pa', displayName: 'Pressure' },
	temperature: {
		graph: true,
		unit: '°C',
		displayName: 'Temperature'
	},
	voltage: { graph: true, unit: 'V', displayName: 'Voltage' },
	gas_concentration: {
		graph: true,
		unit: 'ppm',
		displayName: 'Gas Concentration'
	},
	gnss_time: { graph: false, unit: '', displayName: 'Time' },
	gnss_latitude: { graph: false, unit: '°N', displayName: 'Latitude' },
	gnss_longitude: { graph: false, unit: '°E', displayName: 'Longitude' },
	gnss_altitude: { graph: true, unit: 'm', displayName: 'Altitude' },
	gnss_sats: { graph: false, unit: '', displayName: 'Sats' },
	accel_x: { graph: true, unit: 'm/s2', displayName: 'X' },
	accel_y: { graph: true, unit: 'm/s2', displayName: 'Y' },
	accel_z: { graph: true, unit: 'm/s2', displayName: 'Z' },
	orientation_x: { graph: true, unit: '°', displayName: 'X' },
	orientation_y: { graph: true, unit: '°', displayName: 'Y' },
	orientation_z: { graph: true, unit: '°', displayName: 'Z' },
	software_state: {
		graph: false,
		unit: '',
		displayName: 'Software State'
	},
	velocity: {
		graph: true,
		unit: 'm/s',
		displayName: 'Descent Velocity'
	}
});

export const payloadDataInfo: Readable<{
	[T in TpayloadField]: {
		graph: boolean;
		unit: string;
		displayName: string;
	};
}> = readable({
	team_id: { graph: false, unit: '', displayName: 'Team ID' },
	packet_count: {
		graph: false,
		unit: '',
		displayName: 'Packet Count'
	},
	altitude: { graph: true, unit: 'm', displayName: 'Altitude' },
	pressure: { graph: true, unit: 'Pa', displayName: 'Pressure' },
	temperature: {
		graph: true,
		unit: '°C',
		displayName: 'Temperature'
	},
	gas_concentration: {
		graph: true,
		unit: 'ppm',
		displayName: 'Gas Concentration'
	},
	accel_x: { graph: true, unit: 'm/s2', displayName: 'X' },
	accel_y: { graph: true, unit: 'm/s2', displayName: 'Y' },
	accel_z: { graph: true, unit: 'm/s2', displayName: 'Z' },
	orientation_x: { graph: true, unit: '°', displayName: 'X' },
	orientation_y: { graph: true, unit: '°', displayName: 'Y' },
	orientation_z: { graph: true, unit: '°', displayName: 'Z' },
	velocity: {
		graph: true,
		unit: 'm/s',
		displayName: 'Descent Velocity'
	}
});

export const alltabs: Readable<TtabName[]> = readable(['Overview', 'Graphs', 'CSV']);

export const activeTab: Writable<TtabName> = writable('Overview');

export const rocketData: Writable<TrocketTelemetry> = writable({
	team_id: '2024ASI-ROCKETRY-026',
	time_stamp: '00:00:00',
	packet_count: 0,
	altitude: 0,
	pressure: 0,
	temperature: 0,
	voltage: 0,
	gas_concentration: 0,
	gnss_time: '00:00:00',
	gnss_latitude: 0,
	gnss_longitude: 0,
	gnss_altitude: 0,
	gnss_sats: 0,
	accel_x: 0,
	accel_y: 0,
	accel_z: 0,
	orientation_x: 0,
	orientation_y: 0,
	orientation_z: 0,
	software_state: 'BOOT',
	velocity: 0
});

export const payloadData: Writable<TpayloadTelemetry> = writable({
	team_id: '2024-ASI-ROCKETRY-026',
	packet_count: 0,
	altitude: 0,
	pressure: 0,
	temperature: 0,
	gas_concentration: 0,
	accel_x: 0,
	accel_y: 0,
	accel_z: 0,
	orientation_x: 0,
	orientation_y: 0,
	orientation_z: 0,
	velocity: 0
});

export const timeElapsed = writable('00:00:00');
export const batteryLeft = writable(100);
export const missionProgress = writable(20);
export const initTime = writable(0);
export const nowTime = writable(Date.now());
export const upTime = writable('00:00:00');
export const telemetry_rocket_running = writable(false);
export const telemetry_payload_running = writable(false);
export const sensors_rocket_calibrated = writable(false);
export const sensors_payload_calibrated = writable(false);

export const graphDataTypes__rocket: Readable<TrocketGraphField[]> = readable([
	'altitude',
	'pressure',
	'temperature',
	'voltage',
	'gas_concentration',
	'gnss_altitude',
	'velocity',
	'accel_x',
	'accel_y',
	'accel_y',
	'accel_z',
	'orientation_x',
	'orientation_y',
	'orientation_z'
]);

export const overviewDataTypes__rocket: Readable<TrocketField[]> = readable([
	'team_id',
	'time_stamp',
	'packet_count',
	'altitude',
	'pressure',
	'temperature',
	'voltage',
	'gnss_time',
	'gnss_latitude',
	'gnss_longitude',
	'gnss_altitude',
	'gnss_sats',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'software_state',
	'gas_concentration',
	'velocity'
]);

export const graphDataTypes__payload: Readable<TpayloadGraphField[]> = readable([
	'altitude',
	'pressure',
	'temperature',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'gas_concentration',
	'velocity'
]);

export const overviewDataTypes__payload: Readable<TpayloadField[]> = readable([
	'team_id',
	'packet_count',
	'altitude',
	'pressure',
	'temperature',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'gas_concentration',
	'velocity'
]);

export const rocketPastDatas: Writable<{
	[k in TrocketGraphField]: number[];
}> = writable({
	altitude: [0],
	pressure: [0],
	temperature: [0],
	voltage: [0],
	gas_concentration: [0],
	gnss_altitude: [0],
	velocity: [0],
	accel_x: [0],
	accel_y: [0],
	accel_z: [0],
	orientation_x: [0],
	orientation_y: [0],
	orientation_z: [0]
});

export const payloadPastDatas: Writable<{
	[k in TpayloadGraphField]: number[];
}> = writable({
	altitude: [0],
	pressure: [0],
	temperature: [0],
	gas_concentration: [0],
	velocity: [0],
	accel_x: [0],
	accel_y: [0],
	accel_z: [0],
	orientation_x: [0],
	orientation_y: [0],
	orientation_z: [0]
});

export const csvdata_rocket: Writable<string[]> = writable([]);
export const csvdata_payload: Writable<string[]> = writable([]);

export const simulation_mode = writable(false);
export const travelled_coords: Writable<LatLngExpression[]> = writable([]);
export const GCS_started = writable(false);

export const server_connecting = writable(false);
export const server_disconnecting = writable(false);
export const server_connected = writable(false);
export const server_disconnected = writable(true);

export const last_command: Writable<Tcommand_from_gcs> = writable(null);
