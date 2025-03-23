<script lang="ts">
	import { goto } from '$app/navigation';
	import Button from '@src/lib/components/ui/button/button.svelte';
	import { GCS_started } from '@src/store';

	let startGCS = (e: MouseEvent & { currentTarget: EventTarget & HTMLButtonElement }) => {
		goto('/overview');
		$GCS_started = true;
	};
</script>

<div
	class="bg-accent absolute left-0 top-0 z-[999] grid h-screen w-screen place-content-center transition-all"
>
	<Button
		variant="ghost"
		onclick={startGCS}
		class="hover:text-accent border border-white px-20 py-8 text-lg text-white hover:bg-white active:bg-none"
		>START</Button
	>
</div>

<style lang="scss">
	:global(.starting) {
		transition: all 1000ms linear;
		background-color: #0a0a0a;
	}
</style>
