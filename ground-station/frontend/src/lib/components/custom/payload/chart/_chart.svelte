<script lang="ts">
	import { payloadDataInfo, payloadPastDatas, payloadData } from '@src/store';
	import { Chart, type ChartItem } from 'chart.js/auto';
	import { onDestroy, onMount } from 'svelte';
	import 'chartjs-adapter-luxon';
	import ChartStreaming from 'chartjs-plugin-streaming';

	let {
		datatype,
		subdatatype,
		ratio
	}: { datatype: keyof typeof $allGraphPastDatas; ratio?: number; subdatatype?: string } = $props();

	Chart.register(ChartStreaming);

	let values = $state([0]);

	let i = 0;

	let ctx: ChartItem;
	let chartCanvas: HTMLCanvasElement;
	let chart: Chart<'line', { x: number; y: number }[], string>;

	onMount(() => {
		const context = chartCanvas.getContext('2d');
		if (context) {
			ctx = context;
		}

		chart = new Chart(ctx, {
			type: 'line',

			data: {
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
						},
						type: 'realtime',
						realtime: {
							duration: 20000,
							refresh: 1000,
							delay: 1500
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
	});

	// onDestroy(() => {
	// 	chart.destroy();
	// });

	function addData(dataVar: number) {
		chart.data.datasets.forEach((dataset) => {
			dataset.data.push({ x: Date.now(), y: dataVar });
		});

		chart.update();
	}

	$effect(() => {
		if (datatype in $payloadPastDatas) {
			addData(
				Array.isArray($payloadPastDatas[datatype]) &&
					$payloadDataInfo[datatype].multiParameter == false
					? $payloadPastDatas[datatype].at(0)
					: subdatatype && subdatatype in $payloadPastDatas[datatype]
						? $payloadPastDatas[datatype][subdatatype].at(0)
						: null
			);
		}
	});
</script>

<canvas class="h-full" bind:this={chartCanvas} id={datatype}></canvas>
