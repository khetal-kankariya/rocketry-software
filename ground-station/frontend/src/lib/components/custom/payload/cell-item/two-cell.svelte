<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, payloadDataInfo, payloadData } from '@src/store';
	import Chart from '@src/lib/components/custom/payload/chart/chart.svelte';

	let { datatype }: { datatype: TmultiAxisParams } = $props();

	let datatypes: TmultiAxisDatatypes[] =
		datatype == 'accel'
			? ['accel_x', 'accel_y', 'accel_z']
			: datatype == 'orientation'
				? ['orientation_x', 'orientation_y', 'orientation_z']
				: [];
</script>

<div class="col-span-2 w-auto">
	<Card.Root
		class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[210px]`} w-[688px] cursor-pointer flex-col justify-center gap-2 px-4 py-1 transition-all hover:scale-105`}
	>
		{#if $activeTab == 'Graphs'}
			<div class="graph flex w-full justify-end gap-10">
				{#each datatypes as subdatatype}
					<div class="h-[144px]">
						<Chart ratio={1} datatype={subdatatype}></Chart>
					</div>
				{/each}
			</div>
			<Separator orientation="horizontal" class="w-1/3"></Separator>
		{/if}
		<div class="flex items-center justify-between gap-3">
			<span class="items-center text-nowrap text-base uppercase">
				{datatype}:
			</span>
			<div class={`flex w-full ${datatypes.length == 2 ? 'justify-end' : 'justify-center'} gap-32`}>
				{#each datatypes as subdatatype, i}
					<div class={`flex items-center gap-2`}>
						<span class="text-base uppercase">
							{$payloadDataInfo[subdatatype].displayName}:
						</span>
						<span class="text-3xl">
							{$payloadData[subdatatype] | 0}
						</span>
					</div>
				{/each}
			</div>
		</div>
	</Card.Root>
</div>

<style>
</style>
