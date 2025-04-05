<script lang="ts">
	import { allDataEverything, allGraphPastDatas, readData } from '@src/store';
	import { Chart, type ChartItem } from 'chart.js/auto';
	import { onDestroy, onMount } from 'svelte';

	let { datatype, ratio }: { datatype: keyof TgraphDatatype; ratio?: number } = $props();

	let values = $state([0]);

	let i = 0;

	let ctx: ChartItem;
	let chartCanvas: HTMLCanvasElement;
	let chart: Chart<'line', number[], string>;

	onMount(() => {
		const context = chartCanvas.getContext('2d');
		if (context) {
			ctx = context;
		}

		chart = new Chart(ctx, {
			type: 'line',

			data: {
				labels: [],
				datasets: [
					{
						data: [],
						borderColor: 'rgb(255,255, 255)'
					}
				]
			},
			options: {
				scales: {
					x: {
						border: {
							color: 'rgba(255,255, 255, 0.3)'
						}
					},
					y: {
						border: {
							color: 'rgba(255,255, 255, 0.3)'
						}
					}
				},
				plugins: {
					legend: {
						display: false
					}
				},
				aspectRatio: (ratio || 2.3) / 1
			}
		});

		Array(20).forEach((_, i) => {
			addData(
				Array.isArray($allGraphPastDatas[datatype]) ? $allGraphPastDatas[datatype].at(20 - i) : null
			);
		});
	});

	function addData(dataVar: number) {
		if (chart.data.labels?.length == 20) {
			chart.data.labels.shift();
			chart.data.datasets[0].data.shift();
		}

		if (chart.data.labels) {
			chart.data.labels.push('');
		}
		chart.data.datasets.forEach((dataset) => {
			dataset.data.push(dataVar);
			2;
		});

		chart.update();
	}

	$effect(() => {
		if (datatype in $allGraphPastDatas) {
			addData(
				Array.isArray($allGraphPastDatas[datatype]) ? $allGraphPastDatas[datatype].at(20 - i) : null
			);
		}
	});
</script>

<canvas class="h-full" bind:this={chartCanvas} id={String(datatype)}></canvas>
