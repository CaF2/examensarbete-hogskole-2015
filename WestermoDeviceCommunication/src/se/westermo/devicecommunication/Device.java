/*
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* Originally part of the demo-code from redbearlabs */

package se.westermo.devicecommunication;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class Device extends Activity implements OnItemClickListener {

	private ArrayList<BluetoothDevice> devices;
	private List<Map<String, String>> listItems = new ArrayList<Map<String, String>>();
	private SimpleAdapter adapter;
	private Map<String, String> map = null;
	private ListView listView;
	public final static String DEVICE_NAME = "name";
	public final static String DEVICE_ADDRESS = "address";
	public static final int RESULT_CODE = 31;
	public final static String EXTRA_DEVICE_ADDRESS = "EXTRA_DEVICE_ADDRESS";
	public final static String EXTRA_DEVICE_NAME = "EXTRA_DEVICE_NAME";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.device_list);

		setTitle("Lista över enheter:");

		listView = (ListView) findViewById(R.id.listView);

		devices = (ArrayList<BluetoothDevice>) Main.mDevices;
		for (BluetoothDevice device : devices) {
			map = new HashMap<String, String>();
			map.put(DEVICE_NAME, device.getName());
			map.put(DEVICE_ADDRESS, device.getAddress());
			listItems.add(map);
		}

		adapter = new SimpleAdapter(getApplicationContext(), listItems,
				R.layout.list_item, new String[] { "name", "address" },
				new int[] { R.id.deviceName, R.id.deviceAddr });
		listView.setAdapter(adapter);
		listView.setOnItemClickListener(this);
	}
	
	/**
		När du trycker på en enhet
	*/
	@Override
	public void onItemClick(AdapterView<?> adapterView, View view,
			int position, long id) {
		HashMap<String, String> hashMap = (HashMap<String, String>) listItems
				.get(position);
		String addr = hashMap.get(DEVICE_ADDRESS);
		String name = hashMap.get(DEVICE_NAME);

		Intent intent = new Intent(Device.this, Chat.class);
		intent.putExtra(EXTRA_DEVICE_ADDRESS, addr);
		intent.putExtra(EXTRA_DEVICE_NAME, name);
		startActivity(intent);
		Main.instance.finish();
		finish();
	}
}
