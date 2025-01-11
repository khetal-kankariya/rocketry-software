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
		| 'gnss'
		| 'accel'
		| 'orientation'
		| 'softwareState'
		| 'gasConcn'
		| 'velocity';

	export type TchartFields =
		| 'teamID'
		| 'timeStamp'
		| 'packetCount'
		| 'altitude'
		| 'pressure'
		| 'temperature'
		| 'voltage'
		| 'time'
		| 'latitude'
		| 'longitude'
		| 'sats'
		| 'X'
		| 'Y'
		| 'Z'
		| 'orientation'
		| 'softwareState'
		| 'gasConcn'
		| 'velocity';

	export type TgnssParams = 'time' | 'lat' | 'lng' | 'alt' | 'sats';
	export type TmultiAxisParams = 'x' | 'y' | 'z';

	export type TallData<T extends TtelemetryDataField> = T extends 'gnss'
		? {
				['gnss' as TtelemetryDataField]: {
					graph: boolean;
					unit: string;
					displayName: string;
					multiParameter: true;

					subParams: {
						time: {
							graph: boolean;
							unit: string;
							displayName: string;
							multiParameter: false;
						};
						lat: {
							graph: boolean;
							unit: string;
							displayName: string;
							multiParameter: false;
						};
						lng: {
							graph: boolean;
							unit: string;
							displayName: string;
							multiParameter: false;
						};
						alt: {
							graph: boolean;
							unit: string;
							displayName: string;
							multiParameter: false;
						};
						sats: {
							graph: boolean;
							unit: string;
							displayName: string;
							multiParameter: false;
						};
					};
				};
			}
		: T extends 'accel' | 'orientation'
			? {
					['accel' | ('orientation' as TtelemetryDataField)]: {
						graph: boolean;
						unit: string;
						displayName: string;
						multiParameter: true;
						subParams: {
							x: {
								graph: boolean;
								unit: string;
								displayName: string;
								multiParameter: false;
							};
							y: {
								graph: boolean;
								unit: string;
								displayName: string;
								multiParameter: false;
							};
							z: {
								graph: boolean;
								unit: string;
								displayName: string;
								multiParameter: false;
							};
						};
					};
				}
			: {
					[key in TtelemetryDataField]: {
						graph: boolean;
						unit: string;
						displayName: string;
						multiParameter: false;
					};
				};
	export type TtabName = 'Overview' | 'Graphs' | 'CSV';

	export type TsoftwareState =
		| 'boot'
		| 'setup'
		| 'launch_ready'
		| 'ascent'
		| 'payload_sep'
		| 'descent'
		| 'impact';

	export type TtelemetryData = {
		teamID: string;
		timeStamp: string;
		packetCount: number;
		altitude: number;
		pressure: number;
		temperature: number;
		voltage: number;
		gnss: {
			time: number;
			lat: number;
			lng: number;
			alt: number;
			sats: number;
		};
		orientation: { x: number; y: number; z: number };
		accel: { x: number; y: number; z: number };
		softwareState: TsoftwareState;
		velocity: number;
		gasConcn: number;
	};
}

export {};
