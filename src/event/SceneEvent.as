package event
{
	import face2wind.event.ParamEvent;

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
		
		public static const REMOVE_PLAYER:String = "REMOVE_PLAYER";
		
		public static const CREATE_MONSTER:String = "CREATE_MONSTER";
		
		public static const REBUILD_MAP:String = "REBUILD_MAP";
		
		public static const SHOW_EXPOLORE_LIST:String = "SHOW_EXPOLORE_LIST";
		
		public static const RESTART_SURVIVAL:String = "RESTART_SURVIVAL";
	}
}