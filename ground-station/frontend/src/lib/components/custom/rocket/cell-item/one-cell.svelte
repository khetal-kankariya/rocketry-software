<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, rocketDataInfo, rocketData } from '@src/store';
	import Chart from '@src/lib/components/custom/rocket/chart/chart.svelte';

	let { datatype }: { datatype: TrocketField } = $props();

	let unit = $rocketDataInfo[datatype].unit;
	let sumdata: string | number | undefined = $state();

	$effect(() => {
		sumdata = $rocketData[datatype];
	});
</script>

<li class="w-auto">
	<Card.Root
		class={`flex  w-[336px] cursor-pointer flex-col justify-center gap-1 p-2 px-1 transition-all hover:scale-105`}
	>
		{#if $activeTab == 'Graphs'}
			<span class="hidden">{(datatype = datatype as TrocketGraphField)}</span>
			<div class="graph h-full w-full bg-none">
				<Chart {datatype}></Chart>
			</div>
			<span class="hidden">{(datatype = datatype as TrocketField)}</span>
			<Separator orientation="horizontal" class="w-1/3"></Separator>
		{/if}
		<div class="flex items-center justify-between gap-3 p-2">
			<span class="text-base uppercase">
				{$rocketDataInfo[datatype].displayName}:
			</span>
			<span class="flex items-baseline gap-2 text-3xl">
				<span class="uppercase" class:text-lg={datatype == 'teamID' || datatype == 'softwareState'}>
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
</li>
