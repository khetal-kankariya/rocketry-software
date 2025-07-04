<script lang="ts">
	let { data }: { data: TrocketData } = $props();
</script>

<tr>
	<td>{data?.teamID}</td>
	<td>{data?.timeStamp}</td>
	<td>{data?.packetCount}</td>
	<td>{data?.altitude}</td>
	<td>{data?.pressure}</td>
	<td>{data?.temperature}</td>
	<td>{data?.voltage}</td>
	<td>{data?.gasConcn}</td>
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
	<td>{data?.softwareState}</td>
	<td>{data?.velocity}</td>
</tr>
