package gui 
{
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import gui.*;
	// simple external interface test
	public class GuiTest5 extends SimpleGui
	{
		public function GuiTest5() 
		{
			super();
			o1(ExternalInterface.available);
		}

		override public function btn1_clickHandler(evtObj:MouseEvent):void {
			
			var res:String = ExternalInterface.call("req", "Hello,World!");
			o2(res);
		}
		override public function btn2_clickHandler(evtObj:MouseEvent):void {
			
		}
	}
}