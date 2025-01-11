<script lang="ts">
	import { Separator } from '$lib/components/ui/separator';
	import { activeTab, allDataEverything, allGraphPastDatas, readData } from '@src/store';

	let { data } = $props();

	let { field_name } = data;

	let values = $state([$allGraphPastDatas[field_name]]);

	import { Chart, type ChartItem } from 'chart.js/auto';
	import { onMount } from 'svelte';
	import 'chartjs-adapter-luxon';
	import { nonpassive } from 'svelte/legacy';
	import Button from '@src/lib/components/ui/button/button.svelte';

	let i = 0;

	let ctx: ChartItem;
	let chartCanvas: HTMLCanvasElement;
	let chart: Chart<'line', number[], number>;

	onMount(() => {
		ctx = chartCanvas.getContext('2d');
		chart = new Chart(ctx, {
			type: 'line',
			options: {
				aspectRatio: 1396 / 400,
				plugins: {
					legend: {
						display: false
					}
				},
				scales: {
					x: {}
				}
			},
			data: {
				labels: $allGraphPastDatas.timeStamp,
				datasets: [{ data: $allGraphPastDatas[field_name], borderColor: 'rgb(255,255, 255)' }]
			}
		});
	});

	function addData(dataVar: number) {
		chart.data.labels?.push($allGraphPastDatas.timeStamp.at(0));
		chart.data.datasets.forEach((dataset) => {
			dataset.data.push(dataVar);
		});
		chart.update();
	}

	$effect(() => {
		addData($allGraphPastDatas[field_name].at(0));
	});

	$effect(() => {
		$activeTab = 'Graphs';
	});
</script>

<div class="flex h-full w-[1396px] flex-col gap-8 p-4">
	<div class="flex items-center justify-between">
		<span class="text-2xl uppercase">{field_name}</span>
		<span class="w-20 text-end text-lg"
			>{$readData[field_name]}
			<span>
				{$allDataEverything[field_name].unit}
			</span>
		</span>
	</div>
	<Separator orientation="horizontal"></Separator>
	<canvas class="w-full" bind:this={chartCanvas} id={field_name}></canvas>
</div>
