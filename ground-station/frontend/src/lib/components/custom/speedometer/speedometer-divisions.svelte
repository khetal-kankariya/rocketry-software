<script lang="ts">
	let { value }: { value: number } = $props();
</script>

<div class="z-10 flex flex-col justify-between">
	{#if value % 10 == 0}
		<div class="h-4 w-[1px] border border-white"></div>
	{:else if value % 5 == 0}
		<div class="h-3 w-[1px] border border-white"></div>
	{:else}
		<div class="h-1.5 w-[1px] border border-white"></div>
	{/if}
	<span class="text-xs">
		<!-- {value % 10} -->
	</span>
</div>
