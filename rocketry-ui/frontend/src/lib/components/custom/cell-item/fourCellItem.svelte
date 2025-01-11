<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, allDataEverything, readData } from '@src/store';
	import Chart from '$lib/components/custom/chart/chart.svelte';

	let { datatype }: { datatype: TtelemetryDataField } = $props();

	let readDataTypeData = $derived($readData ? $readData[datatype] : {});
</script>

<div class="col-span-4 w-auto">
	<Card.Root
		class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[210px]`} w-[1392px] cursor-pointer flex-col justify-center gap-1 px-4 py-1 transition-all hover:scale-105`}
	>
		{#if $activeTab == 'Graphs'}
			<div class="graph grid w-full grid-cols-4">
				{#each Object.keys($allDataEverything[datatype].subParams) as actualDatatype}
					<div>
						<Chart ratio={3} datatype={actualDatatype as TtelemetryDataField}></Chart>
					</div>
				{/each}
			</div>
			<Separator orientation="horizontal" class="w-1/3"></Separator>
		{/if}
		<div class="flex items-center gap-[8rem]">
			<span class="text-base uppercase">{$allDataEverything[datatype].displayName}:</span>
			<div class="flex w-full justify-between">
				{#each Object.keys($allDataEverything[datatype].subParams) as actualDatatype}
					<div class="flex items-center gap-2">
						<span class="text-base uppercase">
							{$allDataEverything[datatype].subParams[actualDatatype].displayName}:
						</span>
						<span class="text-3xl">
							{readDataTypeData[actualDatatype] ? readDataTypeData[actualDatatype] : 0}
							<span class="text-sm">
								<!-- {unit} -->
							</span>
						</span>
					</div>
				{/each}
			</div>
		</div>
	</Card.Root>
</div>

<style>
</style>
