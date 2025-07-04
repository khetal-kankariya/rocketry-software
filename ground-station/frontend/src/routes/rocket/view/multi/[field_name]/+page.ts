import { rocketDataInfo } from '@src/store.js';

export const ssr = false;

export const load = ({ params }) => {
	let { field_name } = params;
	let subfield_names = ['x', 'y', 'z'];
	if (field_name == 'gyroSpinRate') subfield_names = ['roll', 'pitch'];
	return {
		field_name: params.field_name as 'accel' | 'orientation' | 'gyroSpinRate',
		subfield_names: subfield_names as ['x', 'y', 'z'] | ['roll', 'pitch']
	};
};
