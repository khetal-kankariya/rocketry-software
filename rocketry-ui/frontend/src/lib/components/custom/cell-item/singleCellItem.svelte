<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import {
		activeTab,
		allDataEverything,
		allGraphDatatypes,
		allGraphPastDatas,
		readData
	} from '@src/store';
	import Chart from '$lib/components/custom/chart/chart.svelte';

	let { datatype }: { datatype: TtelemetryDataField } = $props();

	let unit = $allDataEverything[datatype].unit;
	let sumdata = $derived($readData ? $readData[datatype] : 0);
</script>

<li class="w-auto">
	<a href={datatype in allGraphDatatypes ? `/view/single/${datatype}` : '#'}>
		<Card.Root
			class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[214px]`} w-[336px] cursor-pointer flex-col justify-center gap-1 p-2 transition-all hover:scale-105`}
		>
			{#if $activeTab == 'Graphs'}
				<div class="graph h-full w-full bg-none">
					<Chart {datatype}></Chart>
				</div>
				<Separator orientation="horizontal" class="w-1/3"></Separator>
			{/if}
			<div class="flex items-center justify-between gap-3 p-2">
				<span class="text-base uppercase">
					{$allDataEverything[datatype].displayName}:
				</span>
				<span class="flex items-baseline gap-2 text-3xl">
					<span
						class="uppercase"
						class:text-lg={datatype == 'teamID' || datatype == 'softwareState'}
					>
						{sumdata}
					</span>
					{#if datatype != 'teamID' && datatype != 'packetCount' && datatype != 'softwareState'}
						<span class="text-sm">
							{unit}
						</span>
					{/if}
				</span>
			</div>
		</Card.Root>
	</a>
</li>
