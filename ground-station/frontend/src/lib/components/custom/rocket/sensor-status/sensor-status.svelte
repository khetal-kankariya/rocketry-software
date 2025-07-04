<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import { Checkbox } from '$lib/components/ui/checkbox';
	import { Button } from '$lib/components/ui/button';
	import { rocketPastDatas, batteryLeft, rocketData, timeElapsed } from '@src/store';

	let sensor_lists = [
		['rtc', 'imu', 'pressure', 'temp'],
		['gnss', 'gas', 'logger', 'voltage']
	];

	let sensor_names = {
		rtc: 'RTC',
		imu: 'IMU',
		pressure: 'Pressure',
		temp: 'TEMPERATURE',
		gnss: 'GNSS',
		gas: 'GAS SENSOR',
		logger: 'LOGGER',
		voltage: 'VOLTAGE'
	};

	let sensor_fields = {
		rtc: 'timeStamp',
		imu: 'orientation',
		pressure: 'pressure',
		temp: 'temperature',
		gnss: 'gnss_altitude',
		gas: 'gasConcn',
		voltage: 'voltage'
	};

	let sensor_working = $state([
		[true, true, true, true],
		[true, true, true, true]
	]);

	const sensor_status = (
		sensor_name: 'rtc' | 'imu' | 'pressure' | 'temp' | 'gnss' | 'logger' | 'gas' | 'voltage'
	) => {
		if ($rocketData[sensor_fields[sensor_name] as TrocketField]) return true;
	};

	let parachute_released = $state(false);
</script>

<div class="flex h-full w-[336px] flex-col gap-1 border bg-transparent p-1">
	<Card.Root class="h-full bg-transparent px-4 py-2">
		<span class="text-lg">SENSOR STATUS</span>
		<div class="grid grid-cols-2 gap-4">
			{#each sensor_lists as sensor_list, i}
				<div class="flex flex-col gap-2">
					{#each sensor_list as sensor, j}
						<div class="flex w-[142px] justify-between gap-2 px-2">
							<span class="text text-sm uppercase">{sensor_names[sensor]}</span>
							<Checkbox class="pointer-events-none" checked={sensor_working[i][j]}></Checkbox>
						</div>
					{/each}
				</div>
			{/each}
		</div>
	</Card.Root>

	<Card.Root class="flex h-full w-full flex-col gap-2 bg-transparent px-4 py-2">
		<span class="text-lg">PARACHUTE STATUS</span>
		<div class="flex w-[142px] justify-between">
			<span class="text-sm"> RELEASED </span>
			<Checkbox class="pointer-events-none" checked={parachute_released}></Checkbox>
		</div>
	</Card.Root>

	<Card.Root class="flex h-full w-full flex-col gap-2 bg-transparent px-4 py-2">
		<div class="flex w-full items-center justify-between gap-2">
			<span class="text-sm"> TIME ELAPSED </span>
			<Button class="h-6 px-2 py-0" variant="outline">{$timeElapsed}</Button>
		</div>

		<div class="flex w-full items-center justify-between gap-2">
			<span class="text-sm"> BATTERY STATUS </span>
			<Button class="h-6 px-2 py-0" variant="outline">{`${$batteryLeft}%`}</Button>
		</div>
	</Card.Root>

	<Card.Root class="flex h-full w-full flex-col gap-2 bg-transparent px-4 py-2">
		<div class="flex w-full items-center gap-2">
			<span class="text-sm"> TEAM ID: </span>
			<span>{$rocketData.teamID}</span>
		</div>

		<div class="flex w-full items-center gap-2">
			<span class="text-sm"> TEAM NAME: </span>
			<span class="h-7 uppercase">Team Dyaus</span>
		</div>
	</Card.Root>
</div>
