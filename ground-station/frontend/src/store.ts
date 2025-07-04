import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const GCStype: Writable<null | 'rocket' | 'payload'> = writable(null);

export const apiconnected = writable(false);

export const rocketAllDataNames: Readable<TrocketField[]> = readable([
	'teamID',
	'timeStamp',
	'packetCount',
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
	'softwareState',
	'gasConcn',
	'velocity'
]);

export const payloadAllDataNames: Readable<TrocketField[]> = readable([
	'teamID',
	'packetCount',
	'altitude',
	'pressure',
	'temperature',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'softwareState',
	'gasConcn',
	'velocity'
]);

export const rocketDataInfo: Readable<{
	[T in TrocketField]: {
		graph: boolean;
		unit: string;
		displayName: string;
	};
}> = readable({
	teamID: { graph: false, unit: '', displayName: 'Team ID' },
	timeStamp: {
		graph: false,
		unit: '',
		displayName: 'Time Stamp'
	},
	packetCount: {
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
	gasConcn: {
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
	softwareState: {
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
	teamID: { graph: false, unit: '', displayName: 'Team ID' },
	packetCount: {
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
	gasConcn: {
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

export const rocketData: Writable<TrocketData> = writable({
	teamID: '2024ASI-ROCKETRY-026',
	timeStamp: '00:00:00',
	packetCount: 0,
	altitude: 0,
	pressure: 0,
	temperature: 0,
	voltage: 0,
	gasConcn: 0,
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
	softwareState: 'BOOT',
	velocity: 0
});

export const payloadData: Writable<TpayloadData> = writable({
	teamID: '2024-ASI-ROCKETRY-026',
	packetCount: 0,
	altitude: 0,
	pressure: 0,
	temperature: 0,
	gasConcn: 0,
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
export const telemetry_running = writable(false);
export const sensors_calibrated = writable(false);

export const graphDataTypes__rocket: Readable<TrocketGraphField[]> = readable([
	'altitude',
	'pressure',
	'temperature',
	'voltage',
	'gasConcn',
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
	'teamID',
	'timeStamp',
	'packetCount',
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
	'softwareState',
	'gasConcn',
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
	'gasConcn',
	'velocity'
]);

export const overviewDataTypes__payload: Readable<TpayloadField[]> = readable([
	'teamID',
	'packetCount',
	'altitude',
	'pressure',
	'temperature',
	'orientation_x',
	'orientation_y',
	'orientation_z',
	'accel_x',
	'accel_y',
	'accel_z',
	'gasConcn',
	'velocity'
]);

export const rocketPastDatas: Writable<{
	[k in TrocketGraphField]: number[];
}> = writable({
	altitude: [0],
	pressure: [0],
	temperature: [0],
	voltage: [0],
	gasConcn: [0],
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
	gasConcn: [0],
	velocity: [0],
	accel_x: [0],
	accel_y: [0],
	accel_z: [0],
	orientation_x: [0],
	orientation_y: [0],
	orientation_z: [0]
});

export const csvdata: Writable<string[]> = writable([]);

export const simulation_mode = writable(false);
export const travelled_coords: Writable<LatLngExpression[]> = writable([]);
export const GCS_started = writable(false);

export const server_connecting = writable(false);
export const server_disconnecting = writable(false);
export const server_connected = writable(false);
export const server_disconnected = writable(true);

export const last_command: Writable<Tcommand_from_gcs> = writable(null);
