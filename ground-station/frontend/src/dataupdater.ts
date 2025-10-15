import {
	rocketPastDatas,
	initTime,
	nowTime,
	rocketData,
	timeElapsed,
	upTime,
	payloadPastDatas,
	payloadData
} from './store';

// Updating data derived from rocketData
rocketData.subscribe((presentData) => {
	// Setting time elapsed
	timeElapsed.set(presentData.time_stamp);

	window.sessionStorage.setItem('latest-rocket-data', JSON.stringify(presentData));

	// updating graph data array
	rocketPastDatas.update((val) => {
		if (val.accel_x.length == 180) val.accel_x.pop();
		if (val.accel_y.length == 180) val.accel_x.pop();
		if (val.accel_z.length == 180) val.accel_z.pop();
		if (val.altitude.length == 180) val.altitude.pop();
		if (val.pressure.length == 180) val.pressure.pop();
		if (val.temperature.length == 180) val.temperature.pop();
		if (val.voltage.length == 180) val.voltage.pop();
		if (val.gas_concentration.length == 180) val.gas_concentration.pop();
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
			gas_concentration: [presentData.gas_concentration, ...val.gas_concentration],
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

// Updating data derived from payloadData
payloadData.subscribe((presentData) => {
	window.sessionStorage.setItem('latest-payload-data', JSON.stringify(presentData));

	// updating graph data array
	payloadPastDatas.update((val) => {
		if (val.accel_x.length == 180) val.accel_x.pop();
		if (val.accel_y.length == 180) val.accel_x.pop();
		if (val.accel_z.length == 180) val.accel_z.pop();
		if (val.altitude.length == 180) val.altitude.pop();
		if (val.pressure.length == 180) val.pressure.pop();
		if (val.temperature.length == 180) val.temperature.pop();
		if (val.gas_concentration.length == 180) val.gas_concentration.pop();
		if (val.velocity.length == 180) val.velocity.pop();
		if (val.orientation_x.length == 180) val.orientation_x.pop();
		if (val.orientation_y.length == 180) val.orientation_y.pop();
		if (val.orientation_z.length == 180) val.orientation_z.pop();

		const newVal = {
			altitude: [presentData.altitude, ...val.altitude],
			pressure: [presentData.pressure, ...val.pressure],
			temperature: [presentData.temperature, ...val.temperature],
			gas_concentration: [presentData.gas_concentration, ...val.gas_concentration],
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

initTime.set(Math.floor(Date.now()));
setInterval(() => {
	nowTime.set(Math.floor(Date.now()));
}, 1000);

initTime.subscribe((initVal) => {
	nowTime.subscribe((val) => {
		let uptime = Math.floor(val - initVal);

		// Setting time elapsed
		let hoursElapsed = Math.floor(uptime / 36000000);
		let minutesElapsed = Math.floor((uptime - 36000000 * hoursElapsed) / 60000);
		let secondsElapsed = Math.floor(
			(uptime - 3600000 * hoursElapsed - 60000 * minutesElapsed) / 1000
		);
		upTime.set(
			`${hoursElapsed < 10 ? `0${hoursElapsed}` : hoursElapsed}:${minutesElapsed < 10 ? `0${minutesElapsed}` : minutesElapsed}:${secondsElapsed < 10 ? `0${secondsElapsed}` : secondsElapsed}`
		);
	});
});
