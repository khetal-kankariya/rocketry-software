<script lang="ts">
	import * as Card from '$lib/components/ui/card';
	import Separator from '$lib/components/ui/separator/separator.svelte';
	import { activeTab, allDataEverything, readData } from '@src/store';
	import Chart from '$lib/components/custom/chart/chart.svelte';

	let subdatatypes: TgnssParams[] = [
		'gnss_latitude',
		'gnss_longitude',
		'gnss_altitude',
		'gnss_time',
		'gnss_sats'
	];
	let graph_subdatatypes: TgnssParams[] = [
		'gnss_latitude',
		'gnss_longitude',
		'gnss_altitude',
		'gnss_time'
	];
</script>

<div class="col-span-4 w-auto">
	<Card.Root
		class={`flex ${$activeTab == 'Overview' ? `h-[66px]` : `h-[210px]`} w-[1392px] cursor-pointer flex-col justify-center gap-1 px-4 py-1 transition-all hover:scale-105`}
	>
		<div class="graph grid w-full grid-cols-4 gap-6" class:hidden={$activeTab != 'Graphs'}>
			{#each graph_subdatatypes as datatype}
				<div>
					<Chart {datatype}></Chart>
				</div>
			{/each}
		</div>
		{#if $activeTab == 'Graphs'}
			<Separator orientation="horizontal" class={`w-1/3`}></Separator>
		{/if}
		<div class="flex items-center gap-[8rem]">
			<span class="text-base uppercase">GNSS:</span>
			<div class="flex w-full justify-between">
				{#each subdatatypes as subdatatype}
					{#if subdatatype != 'gnss_time' || $activeTab != 'Graphs'}
						<div class="flex items-center gap-2">
							<span class="text-base uppercase">
								{$allDataEverything[subdatatype].displayName}:
							</span>
							<span class="text-3xl">
								{$readData[subdatatype]}
								<span class="text-sm">
									<!-- {unit} -->
								</span>
							</span>
						</div>
					{/if}
				{/each}
			</div>
		</div>
	</Card.Root>
</div>

<style>
</style>
