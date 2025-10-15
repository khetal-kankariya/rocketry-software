<script lang="ts">
	import { rocketData } from '@src/store';
	import { cn } from '$lib/utils';
	import * as Card from '$lib/components/ui/card';
	import Textarea from '$lib/components/ui/textarea/textarea.svelte';
	import { activeTab } from '@src/store';

	let i = 0;
	let tableBody: Element;

	let incomingTelemetry: string = $state('');

	$effect(() => {
		incomingTelemetry = $rocketData
			? `${$rocketData.team_id}, ${$rocketData.time_stamp}, ${$rocketData.packet_count}, ${$rocketData.altitude},  ${$rocketData.pressure}, ${$rocketData.temperature}, ${$rocketData.voltage}, ${$rocketData.gnss_time}, ${$rocketData.gnss_altitude}, ${$rocketData.gnss_latitude}, ${$rocketData.gnss_longitude}, ${$rocketData.gnss_sats}, ${$rocketData.orientation_x}, ${$rocketData.orientation_y}, ${$rocketData.orientation_z}, ${$rocketData.accel_x}, ${$rocketData.accel_y}, ${$rocketData.accel_z}, ${$rocketData.software_state}, ${$rocketData.gas_concentration}`
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
