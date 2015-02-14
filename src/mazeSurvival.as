package
{
	import controller.MazeController;
	
	import face2wind.ASBaseLib;
	import face2wind.ASUIComponent;
	import face2wind.lib.Debuger;
	
	import flash.display.Sprite;
	
	[SWF(width = "1000", height = "600", frameRate = "60", backgroundColor = "0xffffff")]
	/**
	 * 
	 * @author face2wind
	 */
	public class mazeSurvival extends Sprite
	{
		public function mazeSurvival()
		{
			ASBaseLib.initialize(this, onStart);
		}
		
		private function onStart():void
		{
			ASUIComponent.initialize();
			Debuger.setModuleDebug( Debuger.BASE_LIB, true);
			
			MazeController.getInstance();
		}
	}
}