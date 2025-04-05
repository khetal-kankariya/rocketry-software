<script lang="ts">
	import Chart from '@src/lib/components/custom/chart/chart.svelte';
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, allDataEverything, readData } from '@src/store';
	import Speedometer from '../../custom/speedometer/speedometer.svelte';

	let { datatype }: { datatype: TtelemetryDataField } = $props();

	let unit = $allDataEverything[datatype].unit;
	let sumdata = $derived($readData ? $readData[datatype] : 0);
</script>

<li class="w-auto">
	<Card.Root
		class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[210px]`} w-[336px] flex-col items-center gap-1 px-4 py-1`}
	>
		{#if $activeTab == 'Graphs'}
			<div class="graph w-full">
				<Chart ratio={5} {datatype}></Chart>
			</div>
			<Separator orientation="horizontal" class="w-1/3"></Separator>
		{/if}
		<div class="flex justify-between gap-4">
			<div class="flex w-full flex-col gap-3">
				<span class="text-sm uppercase">
					{$allDataEverything[datatype].displayName}:
				</span>
				<!-- <Speedometer></Speedometer> -->
			</div>
			<span class="flex items-end gap-1">
				<span class="w-12 text-end text-4xl">
					{sumdata}
				</span>
				<span class="pb-1 text-sm">
					{unit}
				</span>
			</span>
		</div>
	</Card.Root>
</li>
