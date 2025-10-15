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
		| 'team_id'
		| 'time_stamp'
		| 'packet_count'
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
		| 'software_state'
		| 'gas_concentration'
		| 'velocity';

	export type TpayloadField =
		| 'team_id'
		| 'packet_count'
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
		| 'velocity'
		| 'gas_concentration';

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

	export type Tsoftware_state =
		| 'BOOT'
		| 'SETUP'
		| 'LAUNCH_READY'
		| 'ASCENT'
		| 'PAYLOAD_SEP'
		| 'DESCENT'
		| 'IMPACT';

	export type TrocketTelemetry = {
		team_id: string;
		time_stamp: string;
		packet_count: number;
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
		software_state: Tsoftware_state;
		gas_concentration: number;
		velocity: number;
	};

	export type TpayloadTelemetry = {
		team_id: string;
		packet_count: number;
		altitude: number;
		pressure: number;
		temperature: number;
		orientation_x: number;
		orientation_y: number;
		orientation_z: number;
		accel_x: number;
		accel_y: number;
		accel_z: number;
		gas_concentration: number;
		velocity: number;
	};

	export type TrocketGraphField =
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'voltage'
		| 'gas_concentration'
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
		| 'gas_concentration'
		| 'velocity'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z';

	export type Tcommand_from_gcs =
		| null
		| 'telestart'
		| 'telestop'
		| 'calibrate'
		| 'telestart (failed)'
		| 'telestop (failed)'
		| 'calibrate (failed)';

	export type TsimModeCommandRepsonse = {
		status: boolean;
		message: string;
	};

	export type TcommandRepsonse = {
		status: boolean;
		rocket: boolean;
		payload: boolean;
		message: string;
	};
}

export {};
