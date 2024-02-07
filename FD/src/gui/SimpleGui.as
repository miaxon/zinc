package gui
{
	import flash.display.MovieClip;
	import fl.controls.Button;
	import fl.controls.Label;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	

	//
	public class SimpleGui extends Sprite {
		protected var inp:TextField;
		protected var out1:TextField;
		protected var out2:TextField;
		protected var lbl1:Label;
		protected var lbl2:Label;
		protected var lbl3:Label;
		protected var lbl4:Label;
		protected var btn1:Button;
		protected var btn2:Button;
	//	protected var st:Stage;
		public function SimpleGui() {
		//	st = this.stage;
			makeGUI();
		}
		
		private function makeGUI():void
		{
			inp = new TextField();
			addChild(inp);
			inp.x = 10;
			inp.y = 30;
			inp.width = 200;
			inp.height = 30;
			inp.border = true;
			inp.type = "input";
			inp.background = true;
			inp.backgroundColor = 0xffffff;
			lbl1 = new Label();
			addChild(lbl1);
			lbl1.x = 10;
			lbl1.y = 10;
			lbl1.width = 50;
			lbl1.height = 20;
			lbl1.text = "input:";
			//
			out1 = new TextField();
			addChild(out1);
			out1.x = 10;
			out1.y = 100;
			out1.width = 240;
			out1.height = 200;
			out1.border = true;
			out1.type = "dynamic";
			out1.background = true;
			out1.backgroundColor = 0xffffff;
			out1.wordWrap = true;
			lbl2 = new Label();
			addChild(lbl2);
			lbl2.x = 10;
			lbl2.y = 80;
			lbl2.width = 50;
			lbl2.height = 20;
			lbl2.text = "out_1:";
			//
			out2 = new TextField();
			addChild(out2);
			out2.x = 300;
			out2.y = 100;
			out2.width = 240;
			out2.height = 200;
			out2.border = true;
			out2.type = "dynamic";
			out2.background = true;
			out2.backgroundColor = 0xffffff;
			out2.wordWrap = true;
			lbl3 = new Label();
			addChild(lbl3);
			lbl3.x = 300;
			lbl3.y = 80;
			lbl3.width = 50;
			lbl3.height = 20;
			lbl3.text = "out_2:";
			//
			lbl4 = new Label();
			addChild(lbl4);
			lbl4.x = 510;
			lbl4.y = 530;
			lbl4.width = 40;
			lbl4.height = 15;
			lbl4.text = "AS 3.0";
			//
			btn1 = new Button();
			addChild(btn1);
			btn1.x = 80;
			btn1.y = 350;
			btn1.label = "btn_1";
			//
			btn2 = new Button();
			addChild(btn2);
			btn2.x = 350;
			btn2.y = 350;
			btn2.label = "btn_2";
			//
			btn1.addEventListener(MouseEvent.CLICK, btn1_clickHandler);
			btn2.addEventListener(MouseEvent.CLICK, btn2_clickHandler);
			btn1.addEventListener(MouseEvent.ROLL_OVER, btn1_rolloverHandler);
			btn1.addEventListener(MouseEvent.ROLL_OUT, btn1_rolloutHandler);
			//out1.doubleClickEnabled = true;
			//out1.addEventListener(MouseEvent.DOUBLE_CLICK, mHandler);
		}
		private function mHandler(e:MouseEvent):void 
		{
			o1(e.type);
		}
		public function btn1_clickHandler(evtObj:MouseEvent):void {}
			
		public function btn2_clickHandler(evtObj:MouseEvent):void { }
		public function btn1_rolloverHandler(evtObj:MouseEvent):void { }
		public function btn1_rolloutHandler(evtObj:MouseEvent):void { }
		public function o1(str:*):void 
		{
			out1.text = str.toString() + "\n" + out1.text;
		
		}
		public function o2(str:*):void 
		{
			out2.text = str.toString() + "\n" + out2.text;
		
		}
	}
	
}