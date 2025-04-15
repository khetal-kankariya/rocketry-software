// See https://kit.svelte.dev/docs/types#app
// for information about these interfaces
declare global {
	namespace App {}

	export type Tstartupdata = {
		startTime: number;
		portList: string[];
		selectedPort: string;
		baudRateList: number[];
		selectedBaudRate: number;
	};

	export type TtelemetryDataField =
		| 'teamID'
		| 'timeStamp'
		| 'packetCount'
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'voltage'
		| 'gnss_time'
		| 'gnss_latitude'
		| 'gnss_longitude'
		| 'gnss_altitude'
		| 'gnss_sats'
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
		| 'softwareState'
		| 'gasConcn'
		| 'velocity';

	export type TgnssParams =
		| 'gnss_time'
		| 'gnss_latitude'
		| 'gnss_longitude'
		| 'gnss_altitude'
		| 'gnss_sats';

	export type TmultiAxisDatatypes = (
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
	) &
		Partial<TtelemetryDataField>;
	export type TmultiAxisParams = 'orientation' | 'accel';

	export type TtabName = 'Overview' | 'Graphs' | 'CSV';

	export type TsoftwareState =
		| 'BOOT'
		| 'SETUP'
		| 'LAUNCH_READY'
		| 'ASCENT'
		| 'PAYLOAD_SEP'
		| 'DESCENT'
		| 'IMPACT';

	export type TtelemetryData = {
		teamID: string;
		timeStamp: string;
		packetCount: number;
		altitude: number;
		pressure: number;
		temperature: number;
		voltage: number;
		gnss_time: string;
		gnss_latitude: number;
		gnss_longitude: number;
		gnss_altitude: number;
		gnss_sats: number;
		orientation_x: number;
		orientation_y: number;
		orientation_z: number;
		accel_x: number;
		accel_y: number;
		accel_z: number;
		softwareState: TsoftwareState;
		velocity: number;
		gasConcn: number;
	};

	export type TgraphDatatype =
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'voltage'
		| 'gasConcn'
		| 'gnss_altitude'
		| 'velocity'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z';

	export type Tcommand_from_gcs = null | 'telestart' | 'telestop' | 'calibrate';
}

export {};
