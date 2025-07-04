<script lang="ts">
	import { LoaderCircle } from 'lucide-svelte';
	import Button from '$lib/components/ui/button/button.svelte';
	import * as Card from '$lib/components/ui/card';
	import { calibrate_sensors, toggle_telemetry } from '@src/socket-interface';
	import { sensors_calibrated, telemetry_running } from '@src/store';

	let toggleTelemetry = () => {
		toggle_telemetry($telemetry_running);
	};

	let calibrationState: 'yes' | 'process' | 'no' = $state('no');

	let startCalibration = async () => {
		calibrationState = 'process';
		calibrationState = (await calibrate_sensors()) ? 'yes' : 'no';
	};
</script>

<div class="command-sender flex w-full gap-4">
	<Card.Root class="flex w-full flex-col gap-2 p-2 text-center">
		<span>TELEMETRY</span>
		{#if !$telemetry_running}
			<Button
				variant="destructive"
				class="w-full text-sm hover:bg-red-800"
				onclick={toggleTelemetry}>STOPPED</Button
			>
		{:else}
			<Button
				variant="default"
				class="w-40full bg-green-600 text-sm hover:bg-green-200"
				onclick={toggleTelemetry}>RUNNING</Button
			>
		{/if}
	</Card.Root>
	<Card.Root class="flex w-full flex-col gap-2 p-2 text-center">
		<span>CALIBRATION</span>
		{#if calibrationState == 'process'}
			<Button disabled>
				<LoaderCircle class="animate-spin">In Process</LoaderCircle></Button
			>
		{:else if !$sensors_calibrated}
			<Button
				variant="destructive"
				class="w-full text-sm hover:bg-red-800"
				onclick={startCalibration}>PENDING</Button
			>
		{:else}
			<Button variant="default" class="w-full bg-green-600 text-sm hover:bg-green-200">DONE</Button>
		{/if}
	</Card.Root>
</div>
