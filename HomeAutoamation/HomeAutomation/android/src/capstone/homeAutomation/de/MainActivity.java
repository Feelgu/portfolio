package capstone.homeAutomation.de;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements View.OnClickListener {

	// 메시지 정수
	public static final int MSG_STATE_CHANGE = 1;
	public static final int MSG_READ = 2;

	// 요청 정수
	private static final int RQ_CONNECT_DEVICE = 1;
	private static final int RQ_ENABLE_BT = 2;

	// 블루투스
	private BluetoothAdapter btAdapter;
	private Service BtService;

	// UI
	private RelativeLayout layout;
	private Button button_1, button_2, button_3, button_4, button_5;
	private TextView text_1, text_2, text_3, text_4, text_5;

	int bluetooth_on_off = 0;

	String read_msg;// 수신받은 데이터를 저장할 변수
	String[] v;// 파싱한 데이터를 저장할 변수
	int dust;// 먼지 데이터를 저장할 변수

	Handler myHandler;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// 스마트폰 가로의 전체길이
		int w = getWindow().getWindowManager().getDefaultDisplay().getWidth();
		// 스마트폰 세로의 전체길이
		int h = getWindow().getWindowManager().getDefaultDisplay().getHeight();

		// 레이아웃의 생성
		layout = new RelativeLayout(this);
		layout.setBackgroundColor(Color.parseColor("#15DC6089"));
		setContentView(layout);

		// 상단의 물건찾기 텍스트뷰 생성
		text_1 = new TextView(this);
		text_1.setId(101);// 텍스트뷰 id는 101
		text_1.setBackgroundResource(R.drawable.rounded_corner1);
		RelativeLayout.LayoutParams param101 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param101.addRule(RelativeLayout.ALIGN_PARENT_TOP, 101);// 위치는 맨위에
		param101.addRule(RelativeLayout.CENTER_HORIZONTAL, 101);// 그리고 가운데
		param101.height = (h / 100) * 10;// 세로 크기는 세로 전체 크기 /100 * 10
		param101.width = (w / 100) * 70;// 가로 크기는 가로 전체 크기 / 100 * 60
		param101.setMargins(0, (h / 100) * 10, 0, 0);// 텍스트뷰 상단에 세로 10:1 크기의 마진
		text_1.setGravity(Gravity.CENTER);// 글자 위치는 가운데
		text_1.setText("Check Seek");
		text_1.setTextSize(30);// 글자 크기
		text_1.setTextColor(Color.parseColor("#000000"));// 글자 색상은 검정색
		text_1.setLayoutParams(param101);
		layout.addView(text_1);

		// 진동 텍스트뷰 생성
		text_3 = new TextView(this);
		text_3.setId(104);
		RelativeLayout.LayoutParams param104 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param104.addRule(RelativeLayout.BELOW, 101);
		param104.addRule(RelativeLayout.ALIGN_PARENT_LEFT, 104);
		param104.height = (h / 100) * 9;
		param104.width = (w / 100) * 30;
		param104.setMargins((w / 100) * 10, (h / 100) * 7, 0, 0);
		text_3.setGravity(Gravity.CENTER);
		text_3.setText("진동");
		text_3.setTextSize(28);
		text_3.setTextColor(Color.parseColor("#000000"));
		text_3.setLayoutParams(param104);
		layout.addView(text_3);

		// 진동 온 버튼 생성
		button_2 = new Button(this);
		button_2.setId(2);
		button_2.setBackgroundResource(R.drawable.rounded_corner1);
		RelativeLayout.LayoutParams param2 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param2.addRule(RelativeLayout.BELOW, 101);
		param2.addRule(RelativeLayout.RIGHT_OF, 104);
		param2.height = (h / 100) * 8;
		param2.width = (w / 100) * 20;
		param2.setMargins((w / 100) * 8, (h / 100) * 7, 0, 0);
		button_2.setGravity(Gravity.CENTER);
		button_2.setText("ON");
		button_2.setTextSize(25);
		button_2.setTextColor(Color.parseColor("#000000"));
		button_2.setLayoutParams(param2);
		button_2.setOnClickListener(this);
		layout.addView(button_2);

		// 진동 오프 버튼 생성
		button_3 = new Button(this);
		button_3.setId(3);
		button_3.setBackgroundResource(R.drawable.rounded_corner2);
		RelativeLayout.LayoutParams param3 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param3.addRule(RelativeLayout.BELOW, 101);
		param3.addRule(RelativeLayout.RIGHT_OF, 2);
		param3.height = (h / 100) * 8;
		param3.width = (w / 100) * 20;
		param3.setMargins((w / 100) * 4, (h / 100) * 7, 0, 0);
		button_3.setGravity(Gravity.CENTER);
		button_3.setText("OFF");
		button_3.setTextSize(25);
		button_3.setTextColor(Color.parseColor("#000000"));
		button_3.setLayoutParams(param3);
		button_3.setOnClickListener(this);
		layout.addView(button_3);

		// 부저 텍스트뷰 생성
		text_4 = new TextView(this);
		text_4.setId(105);
		RelativeLayout.LayoutParams param105 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param105.addRule(RelativeLayout.BELOW, 104);
		param105.addRule(RelativeLayout.ALIGN_PARENT_LEFT, 105);
		param105.height = (h / 100) * 8;
		param105.width = (w / 100) * 30;
		param105.setMargins((w / 100) * 10, (h / 100) * 4, 0, 0);
		text_4.setGravity(Gravity.CENTER);
		text_4.setText("부저");
		text_4.setTextSize(28);
		text_4.setTextColor(Color.parseColor("#000000"));
		text_4.setLayoutParams(param105);
		layout.addView(text_4);

		// 부저 온 버튼 생성
		button_4 = new Button(this);
		button_4.setId(4);
		button_4.setBackgroundResource(R.drawable.rounded_corner1);
		RelativeLayout.LayoutParams param4 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param4.addRule(RelativeLayout.BELOW, 104);
		param4.addRule(RelativeLayout.RIGHT_OF, 105);
		param4.height = (h / 100) * 8;
		param4.width = (w / 100) * 20;
		param4.setMargins((w / 100) * 8, (h / 100) * 4, 0, 0);
		button_4.setGravity(Gravity.CENTER);
		button_4.setText("ON");
		button_4.setTextSize(25);
		button_4.setTextColor(Color.parseColor("#000000"));
		button_4.setLayoutParams(param4);
		button_4.setOnClickListener(this);
		layout.addView(button_4);

		// 부저 오프 버튼 생성
		button_5 = new Button(this);
		button_5.setId(5);
		button_5.setBackgroundResource(R.drawable.rounded_corner2);
		RelativeLayout.LayoutParams param5 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param5.addRule(RelativeLayout.BELOW, 104);
		param5.addRule(RelativeLayout.RIGHT_OF, 4);
		param5.height = (h / 100) * 8;
		param5.width = (w / 100) * 20;
		param5.setMargins((w / 100) * 4, (h / 100) * 4, 0, 0);
		button_5.setGravity(Gravity.CENTER);
		button_5.setText("OFF");
		button_5.setTextSize(25);
		button_5.setTextColor(Color.parseColor("#000000"));
		button_5.setLayoutParams(param5);
		button_5.setOnClickListener(this);
		layout.addView(button_5);

		// 미세먼지 농도 텍스트뷰 생성
		text_5 = new TextView(this);
		text_5.setId(106);
		text_5.setBackgroundResource(R.drawable.rounded_corner3);
		RelativeLayout.LayoutParams param106 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		param106.addRule(RelativeLayout.BELOW, 5);
		param106.addRule(RelativeLayout.CENTER_HORIZONTAL, 106);
		param106.height = (h / 100) * 10;
		param106.width = (w / 100) * 80;
		param106.setMargins(0, (h / 100) * 12, 0, 0);
		text_5.setGravity(Gravity.CENTER);
		text_5.setText("미세먼지 농도: 0");
		text_5.setTextSize(30);
		text_5.setTextColor(Color.parseColor("#FFFFFF"));
		text_5.setLayoutParams(param106);
		layout.addView(text_5);

		// 블루투스 접속상태 텍스트뷰 생성
		text_2 = new TextView(this);
		text_2.setId(102);
		text_2.setTextSize(20);
		text_2.setGravity(Gravity.CENTER);
		text_2.setTextColor(Color.parseColor("#80000000"));
		RelativeLayout.LayoutParams param102 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		param102.addRule(RelativeLayout.LEFT_OF, 1);
		param102.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 102);
		param102.setMargins(0, 0, h / 40, h / 40);
		param102.height = h / 15;
		text_2.setLayoutParams(param102);
		layout.addView(text_2);

		// 블루투스 연결 버튼 생성
		button_1 = new Button(this);
		button_1.setId(1);
		button_1.setBackgroundResource(R.drawable.set);
		RelativeLayout.LayoutParams param1 = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		param1.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, 1);
		param1.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
		param1.setMargins(0, 0, h / 40, h / 40);
		param1.height = h / 15;
		param1.width = h / 15;
		button_1.setLayoutParams(param1);
		button_1.setOnClickListener(this);
		layout.addView(button_1);

		// Bluetooth 어댑터
		btAdapter = BluetoothAdapter.getDefaultAdapter();

	}

	// 어플리케이션 시작 시 불린다.
	@Override
	public void onStart() {
		super.onStart();
		if (!btAdapter.isEnabled()) {
			Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, RQ_ENABLE_BT);
		} else {
			if (BtService == null)
				BtService = new Service(this, handler);
		}
	}

	// 어플리케이션 재개 시 불린다.
	@Override
	public synchronized void onResume() {
		super.onResume();
		if (BtService != null) {
			if (BtService.getState() == Service.STATE_NONE) {
				// Bluetooth의 접속 대기(서버)
				BtService.start();
			}
		}
	}

	// 어플리케이션 파괴 시 불린다.
	@Override
	public void onDestroy() {
		super.onDestroy();
		if (BtService != null)
			BtService.stop();
	}

	// 채팅 서버로부터 정보를 취득하는 핸들러
	private final Handler handler = new Handler() {
		// 핸들 메시지
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
				case MSG_STATE_CHANGE:
					switch (msg.arg1) {
						case Service.STATE_CONNECTED:
							text_2.setText("접속상태: 접속 완료");
							bluetooth_on_off = 1;
							break;
						case Service.STATE_CONNECTING:
							text_2.setText("접속상태: 접속 중");
							break;
						case Service.STATE_LISTEN:
						case Service.STATE_NONE:
							text_2.setText("접속상태: 미접속");
							bluetooth_on_off = 0;
							break;
					}
					break;
				// 메시지 수신 부분
				case MSG_READ:
					byte[] readBuf = (byte[]) msg.obj;
					String readMessage = new String(readBuf, 0, msg.arg1);

					// read_msg 변수에 들어온 데이터를 쌓는다
					read_msg = read_msg + readMessage;

					try {
						if (read_msg.length() < 8) {// 만약 read_msg의 길이가 8보다 작으면
							int l = read_msg.length();// read_msg의 길이를 저장
							// 만약 처음에 위치한 데이터가 @이고, 마지막에 위치한 데이터가 #이면
							if (read_msg.charAt(0) == '@' && read_msg.charAt(l - 1) == '#') {
								// v라는 변수에 @값 사이에있는 데이터를 파싱한다.
								v = read_msg.split("\\@");
								dust = Integer.parseInt(v[1]);// 먼지데이터 저장
								text_5.setText("미세먼지 농도: " + dust);
								read_msg = "";
							}
						} else {
							// read_msg길이가 8이거나 8보다크면 (데이터 수신 에러)
							read_msg = "";// 초기화
						}
					} catch (Exception e) {
						read_msg = "";
					}

					break;
			}
		}
	};

	// 어플리케이션 복귀 시 불린다.
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
			// 단말기 검색
			case RQ_CONNECT_DEVICE:
				if (resultCode == Activity.RESULT_OK) {
					String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
					// Bluetooth의 접속 요구(클라이언트)
					BluetoothDevice device = btAdapter.getRemoteDevice(address);
					BtService.connect(device);
				}
				break;
			// 검색 유효
			case RQ_ENABLE_BT:
				if (resultCode == Activity.RESULT_OK) {
					BtService = new Service(this, handler);
				} else {
					Toast.makeText(this, "Bluetooth가 유효하지 않습니다", Toast.LENGTH_SHORT).show();
					finish();
				}
		}
	}

	// 종료 버튼을 눌렀을때
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
			case KeyEvent.KEYCODE_BACK:

				new AlertDialog.Builder(this).setMessage("\n프로그램을 종료 하시겠습니까?\n").setPositiveButton("예", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						android.os.Process.killProcess(android.os.Process.myPid());
					}
				}).setNegativeButton("아니오", null).show();
				break;
			default:
				break;
		}
		return super.onKeyDown(keyCode, event);
	}

	public void send_data(final String s) {
		if (bluetooth_on_off == 1) {// 블루투스가 연결 되었을때
			BtService.write(s.getBytes());// 입력받은 데이터를 전송한다.
			Handler hd = new Handler();
			hd.postDelayed(new Runnable() {
				@Override
				public void run() {
					BtService.write(s.getBytes());// 한번더 보낸다.
				}
			}, 50);// 0.05초 뒤에
		} else {// 연결 안되었을때
			Toast.makeText(this, "Bluetooth가 연결되어있지 않습니다.", Toast.LENGTH_SHORT).show();
		}
	}

	public void onClick(View v) {

		if (v == button_1) {// 블루투스 연결 버튼을 누르면 DeviceListActivity 실행
			Intent serverIntent = new Intent(this, DeviceListActivity.class);
			startActivityForResult(serverIntent, RQ_CONNECT_DEVICE);
		}

		if (v == button_2) {// 진동 온 버튼
			button_2.setBackgroundResource(R.drawable.rounded_corner2);
			button_3.setBackgroundResource(R.drawable.rounded_corner1);
			send_data("a");// 블루투스로 데이터 a 전송
		}
		if (v == button_3) {// 진동 오프 버튼
			button_2.setBackgroundResource(R.drawable.rounded_corner1);
			button_3.setBackgroundResource(R.drawable.rounded_corner2);
			send_data("b");// 블루투스로 데이터 b 전송
		}

		if (v == button_4) {// 부저 온 버튼
			button_4.setBackgroundResource(R.drawable.rounded_corner2);
			button_5.setBackgroundResource(R.drawable.rounded_corner1);
			send_data("c");
		}
		if (v == button_5) {// 부저 오프 버튼
			button_4.setBackgroundResource(R.drawable.rounded_corner1);
			button_5.setBackgroundResource(R.drawable.rounded_corner2);
			send_data("d");
		}

	}
}
