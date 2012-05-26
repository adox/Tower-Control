function jGaugeControl() {
	this.id = null;
	this.jGauge = null;
	this.rangeStart = 0;
	this.rangeStop = 359;
	this.rangeRestrictUp = 200;
	this.rangeRestrictDown = 160;
	this.init = init;
	this.setValue = setValue;
	this.getValue = getValue;
}

function init() {
	$('#'+this.id).append('<input type="button" value="-15" style="width: 40px" onclick="' + this.id + '.setValue(' + this.id + '.getValue() - 15);" />');
	$('#'+this.id).append('<input type="button" value="-1" style="width: 40px" onclick="' + this.id + '.setValue(' + this.id + '.getValue() - 1);" />');
	$('#'+this.id).append('<input type="button" value="+1" style="width: 40px" onclick="' + this.id + '.setValue(' + this.id + '.getValue() + 1);" />');
	$('#'+this.id).append('<input type="button" value="+15" style="width: 40px" onclick="' + this.id + '.setValue(' + this.id + '.getValue() + 15);" />');

	$('#'+this.id).append('<br/><input type="button" value="Reset" style="width: 80px" onclick="' + this.id + '.setValue(0);" />');	
}

function setValue(d) {
	if(d > this.rangeStop)
		d -= 360;
		
	if(d >= this.rangeStart && d <= this.rangeStop) {
		this.jGauge.setValue(d);
		//alert("http://192.168.1.15?" + d);

		/*
		$.get('http://192.168.1.15/', function(data) {
			alert(data);
			});
		}*/
		
		/*
		$.get('http://192.168.1.15/?100', function(data) {
			alert(data);
			});
		}
		*/

/*		
		$.ajax({
			url: 'http://news.bbc.co.uk',
			type: 'GET',
			success: function(res) {
				var headline = $(res.responseText).find('a.tsh').text();
				alert(headline);
			}
		});		
		}	
	*/	
		/*
		$.get('test.html', function(data) {
			alert(data);
			});
		}
		*/
		$.ajax({
			url: 'http://192.168.1.15',
			//url: 'http://google.pl',
			type: 'GET',
			success: function(res) {
				alert(res);
			}
		});		
		}			
}

function getValue() {
	return this.jGauge.value;
}