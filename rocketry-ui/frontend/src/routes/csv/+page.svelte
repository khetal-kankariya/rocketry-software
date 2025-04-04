<script>
	import { get_csv_data } from '@src/socket-interface';
	import { Button } from '@src/lib/components/ui/button';
	import { activeTab, storecsvdata } from '@src/store';

	let csvDataFields = [
		'TEAM ID',
		'TIME STAMP',
		'PACKET COUNT',
		'SOFTWARE STATE',
		'ALTITUDE',
		'PRESSURE',
		'TEMPERATURE',
		'VOLTAGE',
		'GNSS TIME',
		'GNSS LATITUDE',
		'GNSS LONGITUDE',
		'GNSS ALTITUDE',
		'GNSS SATS',
		'ACCEL X',
		'ACCEL Y',
		'ACCEL Z',
		'ORIENTATION X',
		'ORIENTATION Y',
		'ORIENTATION Z',
		'GAS CONCENTRATION',
		'VELOCITY'
	];

	$effect(() => {
		$activeTab = 'CSV';
	});
</script>

<div class="h-full w-full overflow-scroll pr-8">
	<table class="m-0 text-sm" cellpadding="10" cellspacing="10">
		<tbody>
			<tr class="border-y border-white">
				{#each csvDataFields as csvDataField}
					<th class="text-wrap"> {csvDataField}</th>
				{/each}
			</tr>

			{#each $storecsvdata as data}
				<tr>
					{#each data as fielddat}
						<td class="text-nowrap text-center">{fielddat}</td>
					{/each}
				</tr>
			{/each}
		</tbody>
	</table>
</div>
