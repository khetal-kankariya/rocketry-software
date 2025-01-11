<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, allDataEverything, readData } from '@src/store';
	import Chart from '@src/lib/components/custom/chart/chart.svelte';

	let { datatype }: { datatype: TtelemetryDataField } = $props();
	let actualDatatypes = Object.keys($allDataEverything[datatype].subParams);
	let readaDataTypeData = $derived($readData[datatype]);
</script>

<div class="col-span-2 w-auto">
	<a href={`/view/multi/${datatype}`}>
		<Card.Root
			class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[210px]`} w-[688px] cursor-pointer flex-col justify-center gap-2 px-4 py-1 transition-all hover:scale-105`}
		>
			{#if $activeTab == 'Graphs'}
				<div class="graph flex w-full justify-end gap-10">
					{#each actualDatatypes as actualDatatype}
						<div class="h-[144px]">
							<Chart ratio={1} {datatype} subdatatype={actualDatatype}></Chart>
						</div>
					{/each}
				</div>
				<Separator orientation="horizontal" class="w-1/3"></Separator>
			{/if}
			<div class="flex items-center justify-between gap-3">
				<span class="items-center text-nowrap text-base uppercase"
					>{$allDataEverything[datatype].displayName}:</span
				>
				<div
					class={`flex w-full ${actualDatatypes.length == 2 ? 'justify-end' : 'justify-center'} gap-32`}
				>
					{#each actualDatatypes as actualDatatype, i}
						<div class={`flex items-center gap-2`}>
							<span class="text-base uppercase">
								{$allDataEverything[datatype].subParams[actualDatatype].displayName}:
							</span>
							<span class="text-3xl">
								{readaDataTypeData[actualDatatype] | 0}
							</span>
						</div>
					{/each}
				</div>
			</div>
		</Card.Root>
	</a>
</div>

<style>
</style>
