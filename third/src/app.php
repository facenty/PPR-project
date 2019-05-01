#!/usr/bin/php

<?php
	$host = "127.0.0.1";
	$port = "12121";

	if( ! ( $server = stream_socket_server( "tcp://$host:$port", $errno, $errstr ) ) ){
		print "stream_socket_server(): $errstr\n";
		exit( 1 );
	}

	$client = stream_socket_accept( $server );
	print "Accepted" . "\n";


	while( 1 ){ 
		$rcv = stream_socket_recvfrom($client, 2048);
		// print $rcv;
		print $rcv . "\n";
		// print strlen($rcv) . "\n";
	}

	fclose( $client );
	fclose( $server );

?>