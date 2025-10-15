<script lang="ts">
	let { data }: { data: TrocketTelemetry } = $props();
</script>

<tr>
	<td>{data?.team_id}</td>
	<td>{data?.time_stamp}</td>
	<td>{data?.packet_count}</td>
	<td>{data?.altitude}</td>
	<td>{data?.pressure}</td>
	<td>{data?.temperature}</td>
	<td>{data?.voltage}</td>
	<td>{data?.gas_concentration}</td>
	<td>{data?.gnss.time}</td>
	<td>{data?.gnss.lat}</td>
	<td>{data?.gnss.lng}</td>
	<td>{data?.gnss.alt}</td>
	<td>{data?.gnss.sats}</td>
	<td>{data?.accel.x}</td>
	<td>{data?.accel.y}</td>
	<td>{data?.accel.z}</td>
	<td>{data?.orientation.x}</td>
	<td>{data?.orientation.y}</td>
	<td>{data?.orientation.z}</td>
	<td>{data?.software_state}</td>
	<td>{data?.velocity}</td>
</tr>
