import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const apiconnected = writable(false);

export const payloadAllDataNames: Readable<TdataFields[]> = readable([
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

export const payloadDataInfo: Readable<{
	[T in TdataFields]: {
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

export const payloadData: Writable<TpayloadData> = writable({
	teamID: '2024ASI-ROCKETRY-026',
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

export const allTelemetryData: Writable<TpayloadData[]> = writable([]);

export const missionProgress = writable(20);
export const telemetry_running = writable(false);
export const sensors_calibrated = writable(false);

export const allGraphDatatypes: TpayloadGraphType[] = [
	'altitude',
	'pressure',
	'temperature',
	'gasConcn',
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
	[k in TpayloadGraphType]: number[];
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

export const storecsvdata: Writable<string[]> = writable([]);

export const simulation_mode = writable(false);
export const GCS_started = writable(false);
export const server_connected = writable(false);
export const server_connecting = writable(false);
export const last_command: Writable<Tcommand_from_gcs> = writable(null);
