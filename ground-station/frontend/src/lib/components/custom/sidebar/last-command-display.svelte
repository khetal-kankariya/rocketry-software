<script lang="ts">
	import { activeBaudRate, activePort, last_command } from '@src/store';
	import * as Card from '$lib/components/ui/card';
	import Input from '$lib/components/ui/input/input.svelte';
	import { onMount } from 'svelte';

	onMount(() => {
		$last_command = window.sessionStorage.getItem('last-command') as typeof $last_command;
	});
</script>

<Card.Root class="actions flex w-full items-center gap-2 px-4 py-2">
	<span class="">Last Command: </span>
	<Input type="text" class="w-auto font-mono" value={$last_command} readonly />
</Card.Root>
