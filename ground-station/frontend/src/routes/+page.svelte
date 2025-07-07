<script lang="ts">
	import { goto } from '$app/navigation';
	import Button from '@src/lib/components/ui/button/button.svelte';
	import { GCS_started, GCStype } from '@src/store';

	let startGCS = () => {
		$GCS_started = true;
	};

	const startRocket: EventListener = (e) => {
		startGCS();
		$GCStype = 'rocket';
		goto('/rocket/overview');
	};
	const startPayload: EventListener = (e) => {
		startGCS();
		$GCStype = 'payload';
		goto('/payload/overview');
	};

	$effect.root(() => {
		$GCS_started = false;
	});
</script>

<div class="grid h-screen w-screen grid-flow-col place-content-center gap-4 bg-accent">
	<Button
		variant="ghost"
		onclick={startRocket}
		class="border border-white px-20 py-8 text-lg text-white transition-all hover:bg-white hover:text-accent"
		>ROCKET</Button
	>
	<Button
		variant="ghost"
		onclick={startPayload}
		class="border border-white px-20 py-8 text-lg text-white transition-all hover:bg-white hover:text-accent"
		>PAYLOAD</Button
	>
</div>

<style lang="scss">
	// :global(.starting) {
	// 	transition: all 1000ms linear;
	// 	background-color: #0a0a0a;
	// }
</style>
