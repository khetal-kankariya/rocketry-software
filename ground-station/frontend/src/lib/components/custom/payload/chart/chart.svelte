<script lang="ts">
	import { payloadPastDatas } from '@src/store';
	import { Chart, type ChartItem } from 'chart.js/auto';
	import { onDestroy, onMount, untrack } from 'svelte';
	import type { Action } from 'svelte/action';

	let { datatype, ratio }: { datatype: TpayloadGraphField; ratio?: number } = $props();

	let values = $state<number[]>([]);

	let chart: Chart;
	const chartaction: Action<Node & ChartItem> = (node: Node & ChartItem) => {
		const initvalues = $payloadPastDatas[datatype]
			.slice(0, Math.min(20, $payloadPastDatas[datatype].length))
			.reverse();
		const initlabels = Array.from(' '.repeat(Math.min(20, initvalues.length)));

		chart = new Chart<'line', number[], string[]>(node, {
			type: 'line',

			data: {
				labels: initlabels,
				datasets: [
					{
						data: initvalues,
						borderColor: 'rgba(255, 255, 255, 1)',
						pointRadius: 4,
						pointBackgroundColor: 'white'
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
	};

	const filldata = () => {
		values = $payloadPastDatas[datatype].slice(0, Math.min(20, $payloadPastDatas[datatype].length));
		if (values.length) {
			chart.data.labels = Array.from(' '.repeat(Math.min(20, values.length)));
			chart.data.datasets[0].data.concat(values);
			console.log(chart.data.datasets[0].data);
		}
	};

	function addData(dataVar: number) {
		if (chart) {
			const chartObj = chart.data;
			const chartLabels = chartObj.labels;
			const chartData = chartObj.datasets[0].data;

			if (chartLabels) {
				if (chartLabels.length == 20) chartLabels.pop();
				chartLabels.push(' ');
			}
			if (chartData.length == 20) chartData.shift();
			chartData.push(dataVar);
			chart.update();
		}
	}

	$effect.root(() => {
		$effect(() => {
			payloadPastDatas.subscribe((alldata) => {
				values = $payloadPastDatas[datatype].slice(0, 20);
			});
			untrack(() => {
				addData(values.at(0) || 69);
			});
		});
	});
</script>

<canvas class="chart" use:chartaction></canvas>
