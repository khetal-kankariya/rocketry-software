import { onMount } from 'svelte';
import { allGraphPastDatas, initTime, nowTime, readData, timeElapsed, upTime } from './store';

// Updating data derived from readData
readData.subscribe((presentData) => {
	// Setting time elapsed
	timeElapsed.set(presentData.timeStamp);

	window.sessionStorage.setItem('latest-data', JSON.stringify(presentData));

	// updating graph data array
	allGraphPastDatas.update((val) => {
		if (val.accel.x.length == 180) val.accel.x.pop();
		if (val.accel.y.length == 180) val.accel.x.pop();
		if (val.accel.z.length == 180) val.accel.z.pop();
		if (val.timeStamp.length == 180) val.timeStamp.pop();
		if (val.altitude.length == 180) val.altitude.pop();
		if (val.pressure.length == 180) val.pressure.pop();
		if (val.temperature.length == 180) val.temperature.pop();
		if (val.voltage.length == 180) val.voltage.pop();
		if (val.gasConcn.length == 180) val.gasConcn.pop();
		if (val.velocity.length == 180) val.velocity.pop();
		if (val.gnss.alt.length == 180) val.gnss.alt.pop();
		if (val.orientation.x.length == 180) val.orientation.x.pop();
		if (val.orientation.y.length == 180) val.orientation.y.pop();
		if (val.orientation.z.length == 180) val.orientation.z.pop();

		const newVal = {
			timeStamp: [presentData.timeStamp, ...val.timeStamp],
			altitude: [presentData.altitude, ...val.altitude],
			pressure: [presentData.pressure, ...val.pressure],
			temperature: [presentData.temperature, ...val.temperature],
			voltage: [presentData.voltage, ...val.voltage],
			gasConcn: [presentData.gasConcn, ...val.gasConcn],
			velocity: [presentData.velocity, ...val.velocity],
			accel: {
				x: [presentData.accel.x, ...val.accel.x],
				y: [presentData.accel.y, ...val.accel.y],
				z: [presentData.accel.z, ...val.accel.z]
			},
			gnss: {
				alt: [presentData.gnss.alt, ...val.gnss.alt]
			},
			orientation: {
				x: [presentData.orientation.x, ...val.orientation.x],
				y: [presentData.orientation.y, ...val.orientation.y],
				z: [presentData.orientation.z, ...val.orientation.z]
			}
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
