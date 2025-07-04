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

	export type TdataField = TrocketField | TpayloadField;

	export type TrocketField =
		| 'teamID'
		| 'timeStamp'
		| 'packetCount'
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'voltage'
		| TgnssParams
		| TmultiAxisDatatypes
		| 'softwareState'
		| 'gasConcn'
		| 'velocity';

	export type TpayloadField =
		| 'teamID'
		| 'packetCount'
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| TmultiAxisDatatypes
		| 'velocity'
		| 'gasConcn';

	export type TgnssParams =
		| 'gnss_time'
		| 'gnss_latitude'
		| 'gnss_longitude'
		| 'gnss_altitude'
		| 'gnss_sats';

	export type TmultiAxisDatatypes =
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z';

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

	export type TrocketData = {
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
		gasConcn: number;
		velocity: number;
	};

	export type TpayloadData = {
		teamID: string;
		packetCount: number;
		altitude: number;
		pressure: number;
		temperature: number;
		orientation_x: number;
		orientation_y: number;
		orientation_z: number;
		accel_x: number;
		accel_y: number;
		accel_z: number;
		gasConcn: number;
		velocity: number;
	};

	export type TrocketGraphField =
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

	export type TpayloadGraphField =
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'gasConcn'
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
