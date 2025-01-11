import type { LatLngExpression } from 'leaflet';
import { readable, writable, type Readable, type Writable } from 'svelte/store';

export const apiconnected = writable(false);
export const allDataNames: Readable<TtelemetryDataField[]> = readable([
	'teamID',
	'timeStamp',
	'packetCount',
	'softwareState',
	'altitude',
	'pressure',
	'temperature',
	'voltage',
	'gnss',
	'accel',
	'orientation',
	'gasConcn',
	'velocity'
]);

export const allDataEverything = readable({
	teamID: { graph: false, unit: '', displayName: 'Team ID', multiParameter: false },
	timeStamp: {
		graph: false,
		unit: 's',
		displayName: 'Time Stamp',
		multiParameter: false
	},
	packetCount: {
		graph: false,
		unit: '',
		displayName: 'Packet Count',
		multiParameter: false
	},
	altitude: { graph: true, unit: 'm', displayName: 'Altitude', multiParameter: false },
	pressure: { graph: true, unit: 'Pa', displayName: 'Pressure', multiParameter: false },
	temperature: {
		graph: true,
		unit: '°C',
		displayName: 'Temperature',
		multiParameter: false
	},
	voltage: { graph: true, unit: 'V', displayName: 'Voltage', multiParameter: false },
	gasConcn: {
		graph: true,
		unit: 'ppm',
		displayName: 'Gas Concentration',
		multiParameter: false
	},
	gnss: {
		subParams: {
			time: { graph: false, unit: 's', displayName: 'Time', multiParameter: false },
			lat: { graph: false, unit: '°N', displayName: 'Latitude', multiParameter: false },
			lng: { graph: false, unit: '°E', displayName: 'Longitude', multiParameter: false },
			alt: { graph: true, unit: 'm', displayName: 'Altitude', multiParameter: false },
			sats: { graph: false, unit: '', displayName: 'Sats', multiParameter: false }
		},
		graph: true,
		unit: '',
		displayName: 'GNSS',
		multiParameter: true
	},
	accel: {
		subParams: {
			x: { graph: true, unit: 'm/s2', displayName: 'X', multiParameter: false },
			y: { graph: true, unit: 'm/s2', displayName: 'Y', multiParameter: false },
			z: { graph: true, unit: 'm/s2', displayName: 'Z', multiParameter: false }
		},
		graph: true,
		unit: '',
		displayName: 'Acceleration',
		multiParameter: true
	},
	orientation: {
		subParams: {
			x: { graph: true, unit: '°', displayName: 'X', multiParameter: false },
			y: { graph: true, unit: '°', displayName: 'Y', multiParameter: false },
			z: { graph: true, unit: '°', displayName: 'Z', multiParameter: false }
		},
		graph: true,
		unit: '',
		displayName: 'Orientation',
		multiParameter: true
	},

	softwareState: {
		graph: false,
		unit: '',
		displayName: 'Software State',
		multiParameter: false
	},
	velocity: {
		graph: true,
		unit: 'm/s',
		displayName: 'Descent Velocity',
		multiParameter: false
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
	gnss: { time: 0, lat: 0, lng: 0, alt: 0, sats: 0 },
	accel: { x: 0, y: 0, z: 0 },
	orientation: { x: 0, y: 0, z: 0 },
	softwareState: 'boot',
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

export const allGraphPastDatas: Writable<{
	timeStamp: string[];
	altitude: number[];
	pressure: number[];
	temperature: number[];
	voltage: number[];
	gasConcn: number[];
	gnss: {
		alt: number[];
	};
	velocity: number[];
	accel: {
		x: number[];
		y: number[];
		z: number[];
	};
	orientation: {
		x: number[];
		y: number[];
		z: number[];
	};
}> = writable({
	timeStamp: [],
	altitude: [],
	pressure: [],
	temperature: [],
	voltage: [],
	gasConcn: [],
	gnss: {
		alt: []
	},
	velocity: [],
	accel: {
		x: [],
		y: [],
		z: []
	},
	orientation: {
		x: [],
		y: [],
		z: []
	},
	gyroSpinRate: {
		roll: [],
		pitch: []
	}
});

export const storecsvdata: Writable<string[]> = writable([]);

export const simulation_mode = writable(false);
export const travelled_coords: Writable<LatLngExpression[]> = writable([]);
