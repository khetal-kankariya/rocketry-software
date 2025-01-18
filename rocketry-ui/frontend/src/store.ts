import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const apiconnected = writable(false);
export const allDataNames: Readable<TtelemetryDataField[]> = readable([
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

export const allDataEverything: Readable<{
	[T in TtelemetryDataField]: {
		graph: boolean;
		unit: string;
		displayName: string;
	};
}> = readable({
	teamID: { graph: false, unit: '', displayName: 'Team ID' },
	timeStamp: {
		graph: false,
		unit: 's',
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
	gnss_time: { graph: false, unit: 's', displayName: 'Time' },
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

export const alltabs: Readable<TtabName[]> = readable(['Overview', 'Graphs', 'CSV']);

export const activeTab: Writable<TtabName> = writable('Overview');

export const readData: Writable<TtelemetryData> = writable({
	teamID: '2024ASI-ROCKETRY-026',
	timeStamp: '00:00:00',
	packetCount: 0,
	altitude: 0,
	pressure: 0,
	temperature: 0,
	voltage: 0,
	gasConcn: 0,
	gnss_time: 0,
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

export const allTelemetryData: Writable<TtelemetryData[]> = writable([]);

export const timeElapsed = writable('00:00:00');
export const batteryLeft = writable(100);
export const missionProgress = writable(20);
export const initTime = writable(0);
export const nowTime = writable(Date.now());
export const upTime = writable('00:00:00');
export const telemetry_running = writable(false);
export const sensor_calibrated = writable(false);

export const ports: Writable<string[]> = writable([]);
export const activePort: Writable<string> = writable('');
export const baudRates: Writable<number[]> = writable([]);
export const activeBaudRate: Writable<number> = writable();

export const allGraphDatatypes: TgraphDatatype[] = [
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
];

export const allGraphPastDatas: Writable<{
	altitude: number[];
	pressure: number[];
	temperature: number[];
	voltage: number[];
	gasConcn: number[];
	gnss_altitude: number[];
	velocity: number[];
	accel_x: number[];
	accel_y: number[];
	accel_z: number[];
	orientation_x: number[];
	orientation_y: number[];
	orientation_z: number[];
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

export const storecsvdata: Writable<string[]> = writable([]);

export const simulation_mode = writable(true);
export const travelled_coords: Writable<LatLngExpression[]> = writable([]);
