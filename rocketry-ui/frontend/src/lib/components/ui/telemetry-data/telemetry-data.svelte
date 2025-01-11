<script lang="ts">
	import { readData } from '@src/store';
	import { cn } from '$lib/utils';
	import * as Card from '$lib/components/ui/card';
	import Textarea from '$lib/components/ui/textarea/textarea.svelte';
	import { activeTab } from '@src/store';

	let i = 0;
	let tableBody: Element;

	let incomingTelemetry: string = $state('');

	$effect(() => {
		incomingTelemetry = $readData
			? `${$readData.teamID}, ${$readData.timeStamp}, ${$readData.packetCount}, ${$readData.altitude},  ${$readData.pressure}, ${$readData.temperature}, ${$readData.voltage}, ${$readData.gnss.time}, ${$readData.gnss.alt}, ${$readData.gnss.lat}, ${$readData.gnss.lng}, ${$readData.gnss.sats}, ${$readData.accel.x}, ${$readData.accel.y}, ${$readData.accel.z}, ${$readData.orientation.x}, ${$readData.orientation.y}, ${$readData.orientation.z}, ${$readData.softwareState}, ${$readData.gasConcn}`
			: '<TEAM ID>, <TIME STAMPING>, <PACKET COUNT>, <ALTITUDE>, <PRESSURE>, <TEMP>, <VOLTAGE>, <GNSS TIME>, <GNSS LATITUDE>, <GNSS LONGITUDE>, <GNSS ALTITUDE>, <GNSS SATS>, <ACCELEROMETER DATA>, <FLIGHT SOFTWARE STATE>, <ANY OPTIONAL DATA>';
	});
</script>

<div class={cn('flex w-full gap-4 text-white')}>
	<span class="content-center text-xl"> TELEMETRY </span>

	<Card.Root class="h-min w-full p-2">
		<Textarea
			class="text-md min-h-6 max-w-full resize-none border-none bg-transparent p-0  text-white"
			name="raw-telemetry"
			bind:value={incomingTelemetry}
			readonly={true}
		></Textarea>
	</Card.Root>
</div>
