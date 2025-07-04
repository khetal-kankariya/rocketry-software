<script lang="ts">
	import { Separator } from '$lib/components/ui/separator';
	import { activeTab, rocketDataInfo, rocketPastDatas, rocketData } from '@src/store';

	let { data } = $props();

	let { field_name, subfield_names } = data;

	let values = $state([[], [], []]);

	subfield_names.forEach((subfield, i) => {
		values[i].push($rocketPastDatas[field_name][subfield]);
	});

	import { Chart, type ChartItem } from 'chart.js/auto';
	import { onMount } from 'svelte';
	import 'chartjs-adapter-luxon';

	let i = 0;

	let ctx: CanvasRenderingContext2D;
	let chartCanvas1: HTMLCanvasElement;
	let chartCanvas2: HTMLCanvasElement;
	let chartCanvas3: HTMLCanvasElement;
	let chartCanvases: HTMLCanvasElement[] = $state([chartCanvas1, chartCanvas2, chartCanvas3]);
	let chart1: Chart<'line', number[], number>;
	let chart2: Chart<'line', number[], number>;
	let chart3: Chart<'line', number[], number>;
	let charts: Chart<'line', number[], number>[] = $state([chart1, chart2, chart3]);

	onMount(() => {
		chartCanvases.forEach((chartCanvas, i) => {
			ctx = chartCanvas.getContext('2d');
			charts[i] = new Chart(ctx, {
				type: 'line',
				options: {
					// aspectRatio: 1396 / 133,
					plugins: {
						legend: {
							display: false
						}
					}
				},
				data: {
					labels: $rocketPastDatas.timeStamp,
					datasets: [
						{
							data: $rocketPastDatas[field_name][subfield_names[i]],
							borderColor: 'rgb(255,255, 255)'
						}
					]
				}
			});
		});
	});

	function addData(dataVar: number, i: number) {
		if (charts[i].data.labels?.at(-1) != $rocketPastDatas.timeStamp.at(0))
			if (charts[i].data.labels.push($rocketPastDatas.timeStamp.at(0)) == 0) console.log('ERROR');
		if (charts[i].data.datasets[0].data.push(dataVar) == 0) console.log('ERROR');
		charts[i].update();
	}

	$effect(() => {
		subfield_names.forEach((subfield, i) => {
			addData($rocketPastDatas[field_name][subfield].at(0), i);
		});
	});

	$effect(() => {
		$activeTab = 'Graphs';
		console.log(subfield_names);
	});
</script>

<div class="flex h-full w-[1396px] flex-col gap-8 p-4">
	<div class="flex items-center justify-between">
		<span class="text-2xl uppercase">{field_name}</span>
		<div class="flex gap-4">
			{#each subfield_names as subfield, i}
				<div class="flex w-28 gap-1">
					<span class="text-base uppercase">{subfield}:</span>
					<span class="text-end text-base"
						>{$rocketData[field_name][subfield]}
						<span>
							{$rocketDataInfo[field_name].unit}
						</span>
					</span>
				</div>
			{/each}
		</div>
	</div>
	<div class="flex flex-col gap-4">
		{#each subfield_names as subfield, i}
			<Separator orientation="horizontal"></Separator>
			<canvas
				class="sum-chart hover:h-20"
				style="height: 100px;"
				bind:this={chartCanvases[i]}
				id={subfield}
			></canvas>
		{/each}
	</div>
</div>

<style lang="scss">
	.sum-chart {
		&:hover {
			height: 400px;
		}
	}
</style>
