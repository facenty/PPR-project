#!/usr/bin/php

<?php
	$host = "0.0.0.0";
	$port = "12121";

	if( ! ( $server = stream_socket_server( "tcp://$host:$port", $errno, $errstr ) ) ){
		print "stream_socket_server(): $errstr\n";
		exit( 1 );
	}

	if( ! ($client = stream_socket_accept( $server ) ) ) {
		print "stream_socket_accept(): $errstr\n";
		exit( 1 );
	}
	print "Accepted" . "\n";


	while( 1 ){ 
		$rcv = stream_socket_recvfrom($client, 2048);
		print $rcv;
		// print $rcv. "\n";
		// print strlen($rcv) . "\n";
	}

	fclose( $client );
	fclose( $server );

?>
