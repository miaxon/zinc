package gui 
{
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import flash.utils.ByteArray;

	// simple external interface test
	public class GuiTest6 extends SimpleGui
	{
		private var sharedMemBlock:ByteArray = new ByteArray();
		public function GuiTest6() 
		{
			super();
			o1(ExternalInterface.available);
			sharedMemBlock.length = 1024 * 1024;
			eggs(sharedMemBlock);
		}
		private static function eggs(arr:ByteArray):void 
		{
			for (var i:uint = 0; i < 8; i++)
			{
				arr[i] = arr[i+8] = 0x61 + i + i;
			}
			
		}

		override public function btn1_clickHandler(evtObj:MouseEvent):void {
			
			var res:String = ExternalInterface.call("bla");
			o2(sharedMemBlock);
		}
		override public function btn2_clickHandler(evtObj:MouseEvent):void {
			
		}
	}
}