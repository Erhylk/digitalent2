curl --verbose --request POST "http://api.thingspeak.com/update.json" ^
--header "host:api.thingspeak.com" ^
--header "Connection: close" ^
--header "Content-Type: application/x-www-form-urlencode" ^
--data "api_key=YV7XVW7R1BDWJYLF&field1=34&field2=47"