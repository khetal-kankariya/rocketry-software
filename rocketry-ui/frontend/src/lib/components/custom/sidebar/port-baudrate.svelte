<script lang="ts">
	import { activeBaudRate, activePort } from '@src/store';
	import * as Card from '$lib/components/ui/card';
	import Input from '$lib/components/ui/input/input.svelte';

	let selectedPort: string = $state($activePort || 'COM 3');

	let selectedBaudRate: string = $state($activeBaudRate?.toString() || '9600');
</script>

<Card.Root class="actions flex flex-col gap-2 px-4 py-2 text-base">
	<span class="flex w-min items-center gap-4 text-nowrap">
		<span> Serial Port </span>
		<Input class="min-w-40" readonly={true} name="port" bind:value={selectedPort}></Input>
	</span>
	<span class="flex w-min items-center gap-4 text-nowrap">
		<span> Baud Rate </span>
		<Input class="min-w-40" readonly={true} name="baud-rate" bind:value={selectedBaudRate}></Input>
	</span>
</Card.Root>
