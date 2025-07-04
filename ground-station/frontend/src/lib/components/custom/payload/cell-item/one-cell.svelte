<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, payloadDataInfo, payloadData } from '@src/store';
	import Chart from '@src/lib/components/custom/payload/chart/chart.svelte';

	let { datatype }: { datatype: TpayloadField } = $props();

	let unit = $payloadDataInfo[datatype].unit;
	let sumdata = $state($payloadData[datatype]);
</script>

<li class="w-auto">
	<Card.Root
		class={`flex  w-[336px] cursor-pointer flex-col justify-center gap-1 p-2 px-1 transition-all hover:scale-105`}
	>
		{#if $activeTab == 'Graphs'}
			<span class="hidden">
				{(datatype = datatype as TpayloadGraphField | TpayloadGraphField)}
			</span>
			<div class="graph h-full w-full bg-none">
				<Chart {datatype}></Chart>
			</div>
			<Separator orientation="horizontal" class="w-1/3"></Separator>
		{/if}
		<div class="flex items-center justify-between gap-3 p-2">
			<span class="text-base uppercase">
				{$payloadDataInfo[datatype].displayName}:
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
