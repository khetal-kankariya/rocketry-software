<script lang="ts">
	import { payloadData } from '@src/store';
	import { cn } from '$lib/utils';
	import * as Card from '$lib/components/ui/card';
	import Textarea from '$lib/components/ui/textarea/textarea.svelte';
	import { activeTab } from '@src/store';

	let i = 0;
	let tableBody: Element;

	let incomingTelemetry: string = $state('');

	$effect(() => {
		incomingTelemetry = $payloadData
			? `${$payloadData.teamID}, ${$payloadData.timeStamp}, ${$payloadData.packetCount}, ${$payloadData.altitude},  ${$payloadData.pressure}, ${$payloadData.temperature}, ${$payloadData.voltage}, ${$payloadData.gnss_time}, ${$payloadData.gnss_altitude}, ${$payloadData.gnss_latitude}, ${$payloadData.gnss_longitude}, ${$payloadData.gnss_sats}, ${$payloadData.orientation_x}, ${$payloadData.orientation_y}, ${$payloadData.orientation_z}, ${$payloadData.accel_x}, ${$payloadData.accel_y}, ${$payloadData.accel_z}, ${$payloadData.softwareState}, ${$payloadData.gasConcn}`
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
