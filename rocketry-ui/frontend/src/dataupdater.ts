import { onMount } from 'svelte';
import { allGraphPastDatas, initTime, nowTime, readData, timeElapsed, upTime } from './store';

// Updating data derived from readData
readData.subscribe((presentData) => {
	// Setting time elapsed
	timeElapsed.set(presentData.timeStamp);

	window.sessionStorage.setItem('latest-data', JSON.stringify(presentData));

	// updating graph data array
	allGraphPastDatas.update((val) => {
		if (val.accel_x.length == 180) val.accel_x.pop();
		if (val.accel_y.length == 180) val.accel_x.pop();
		if (val.accel_z.length == 180) val.accel_z.pop();
		if (val.altitude.length == 180) val.altitude.pop();
		if (val.pressure.length == 180) val.pressure.pop();
		if (val.temperature.length == 180) val.temperature.pop();
		if (val.voltage.length == 180) val.voltage.pop();
		if (val.gasConcn.length == 180) val.gasConcn.pop();
		if (val.velocity.length == 180) val.velocity.pop();
		if (val.gnss_altitude.length == 180) val.gnss_altitude.pop();
		if (val.orientation_x.length == 180) val.orientation_x.pop();
		if (val.orientation_y.length == 180) val.orientation_y.pop();
		if (val.orientation_z.length == 180) val.orientation_z.pop();

		const newVal = {
			altitude: [presentData.altitude, ...val.altitude],
			pressure: [presentData.pressure, ...val.pressure],
			temperature: [presentData.temperature, ...val.temperature],
			voltage: [presentData.voltage, ...val.voltage],
			gasConcn: [presentData.gasConcn, ...val.gasConcn],
			velocity: [presentData.velocity, ...val.velocity],
			accel_x: [presentData.accel_x, ...val.accel_x],
			accel_y: [presentData.accel_y, ...val.accel_y],
			accel_z: [presentData.accel_z, ...val.accel_z],
			gnss_altitude: [presentData.gnss_altitude, ...val.gnss_altitude],
			orientation_x: [presentData.orientation_x, ...val.orientation_x],
			orientation_y: [presentData.orientation_y, ...val.orientation_y],
			orientation_z: [presentData.orientation_z, ...val.orientation_z]
		};
		return newVal;
	});
});

setInterval(() => {
	nowTime.set(Math.floor(Date.now() / 1000));
}, 1000);

initTime.subscribe((initVal) => {
	nowTime.subscribe((val) => {
		let uptime = Math.floor(val - initVal);

		// Setting time elapsed
		let hoursElapsed = Math.floor(uptime / 3600);
		let minutesElapsed = Math.floor((uptime - 3600 * hoursElapsed) / 60);
		let secondsElapsed = uptime - 3600 * hoursElapsed - 60 * minutesElapsed;
		upTime.set(
			`${hoursElapsed < 10 ? `0${hoursElapsed}` : hoursElapsed}:${minutesElapsed < 10 ? `0${minutesElapsed}` : minutesElapsed}:${secondsElapsed < 10 ? `0${secondsElapsed}` : secondsElapsed}`
		);
	});
});
