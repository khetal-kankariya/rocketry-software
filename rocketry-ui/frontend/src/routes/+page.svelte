<script lang="ts">
	import { goto } from '$app/navigation';
	import Button from '@src/lib/components/ui/button/button.svelte';

	let startGCS = (e: MouseEvent & { currentTarget: EventTarget & HTMLButtonElement }) => {
		
		goto('/overview');
	};
</script>

<div
	class="absolute left-0 top-0 z-[999] grid h-screen w-screen place-content-center bg-accent transition-all"
>
	<Button
		variant="ghost"
		onclick={startGCS}
		class="border border-white px-20 py-8 text-lg text-white hover:bg-white hover:text-accent active:bg-none"
		>START</Button
	>
</div>

<style lang="scss">
	:global(.starting) {
		transition: all 1000ms linear;
		background-color: #0a0a0a;
	}
</style>
