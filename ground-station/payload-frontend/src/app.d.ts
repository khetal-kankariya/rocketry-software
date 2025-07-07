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

	export type TdataFields =
		| 'teamID'
		| 'packetCount'
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
		| 'gasConcn';

	export type TmultiAxisDatatypes = (
		| 'orientation_x'
		| 'orientation_y'
		| 'orientation_z'
		| 'accel_x'
		| 'accel_y'
		| 'accel_z'
	) &
		Partial<TdataFields>;
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
		velocity: number;
		gasConcn: number;
	};

	export type TpayloadGraphType =
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
