import { onMount } from 'svelte';
import { allGraphPastDatas, payloadData } from '@src/store';

// Updating data derived from readData
payloadData.subscribe((presentData) => {
	// Setting time elapsed
	window.sessionStorage.setItem('latest-data', JSON.stringify(presentData));

	// updating graph data array
	allGraphPastDatas.update((val) => {
		if (val.accel_x.length == 180) val.accel_x.pop();
		if (val.accel_y.length == 180) val.accel_x.pop();
		if (val.accel_z.length == 180) val.accel_z.pop();
		if (val.altitude.length == 180) val.altitude.pop();
		if (val.pressure.length == 180) val.pressure.pop();
		if (val.temperature.length == 180) val.temperature.pop();
		if (val.gasConcn.length == 180) val.gasConcn.pop();
		if (val.velocity.length == 180) val.velocity.pop();
		if (val.orientation_x.length == 180) val.orientation_x.pop();
		if (val.orientation_y.length == 180) val.orientation_y.pop();
		if (val.orientation_z.length == 180) val.orientation_z.pop();

		const newVal = {
			altitude: [presentData.altitude, ...val.altitude],
			pressure: [presentData.pressure, ...val.pressure],
			temperature: [presentData.temperature, ...val.temperature],
			gasConcn: [presentData.gasConcn, ...val.gasConcn],
			velocity: [presentData.velocity, ...val.velocity],
			accel_x: [presentData.accel_x, ...val.accel_x],
			accel_y: [presentData.accel_y, ...val.accel_y],
			accel_z: [presentData.accel_z, ...val.accel_z],
			orientation_x: [presentData.orientation_x, ...val.orientation_x],
			orientation_y: [presentData.orientation_y, ...val.orientation_y],
			orientation_z: [presentData.orientation_z, ...val.orientation_z]
		};
		return newVal;
	});
});
