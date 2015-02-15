package event
{
	/**
	 * 场景事件
	 * @author face2wind
	 */
	public class SceneEvent
	{
		public function SceneEvent()
		{
		}
		
		public static const RESTART_DEMO:String = "RESTART_DEMO";
		
		public static const PATH_SHOW_COMPLETE:String = "PATH_SHOW_COMPLETE";
		
		public static const CREATE_PLAYER:String = "CREATE_PLAYER";
		
		public static const CREATE_MONSTER:String = "CREATE_MONSTER";
		
		public static const REBUILD_MAP:String = "REBUILD_MAP";
	}
}