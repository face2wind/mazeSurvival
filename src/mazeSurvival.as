package
{
	import controller.MazeController;
	
	import face2wind.ASBaseLib;
	import face2wind.lib.Debuger;
	
	import flash.display.Sprite;
	
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
			Debuger.setModuleDebug( Debuger.BASE_LIB, true);
			
			MazeController.getInstance();
		}
	}
}