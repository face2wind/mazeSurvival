package view
{
	import event.SceneEvent;
	
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.manager.TimerManager;
	import face2wind.view.BaseSprite;
	
	import findPath.AStarPathFinder;
	
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.geom.Point;
	
	import model.SceneManager;
	
	/**
	 * 场景
	 * @author face2wind
	 */
	public class Scene extends BaseSprite
	{
		/**
		 * 网格 
		 */		
		private var meshShape:Shape;
		
		private var manager:SceneManager;
		private var pathFinder:AStarPathFinder;
		private var elementLayer:BaseSprite;
		private var pathLayer:BaseSprite;
		
		/**
		 * 临时变量，寻好的待显示路径 
		 */		
		private var _tmpFindPathArr:Array;
		
		public function Scene()
		{
			super();
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			meshShape = new Shape();
			addChild(meshShape);
			elementLayer = new BaseSprite();
			addChild(elementLayer);
			pathLayer = new BaseSprite();
			addChild(pathLayer);
			
			manager = SceneManager.getInstance();
			pathFinder = new AStarPathFinder();
		}
		
		/**
		 * 清空地图 
		 */		
		private function clearMaze():void
		{
			meshShape.graphics.clear();
			
		}
		
		/**
		 * 初始化地图 
		 */		
		private function initMaze():void
		{
			var mapData:Array = manager.getMapData();
			if(null == mapData || 0 == mapData.length)
				return;
			
			pathFinder.setMapData(mapData);
			elementLayer.removeAllChildren();
			var g:Graphics = meshShape.graphics;
			g.clear();
			g.lineStyle(1,0xff00ff);
			for (var i:int = 0; i <= mapData[0].length; i++) {
				g.moveTo(0, i*MapItem.HEIGHT);
				g.lineTo(MapItem.WIDTH*(mapData.length),  i*MapItem.HEIGHT);
			}
			for (var j:int = 0; j <= mapData.length; j++) {
				g.moveTo(j*MapItem.WIDTH, 0);
				g.lineTo(j*MapItem.WIDTH, MapItem.HEIGHT*(mapData[0].length));
			}
			for (var k:int = 0; k < mapData.length; k++) {
				for (var l:int = 0; l < mapData[0].length; l++){
					var mapItem:MapItem = new MapItem();
					mapItem.type = mapData[k][l];
					mapItem.move(k*MapItem.WIDTH, l*MapItem.HEIGHT);
					elementLayer.addChild(mapItem);
				}
			}
			
		}
		
		private function clearPath():void
		{
			pathLayer.removeAllChildren();
		}
		
		/**
		 * 测试寻路并把路径画出来 
		 */		
		private function showFindPathTimer():void
		{
			var mapData:Array = manager.getMapData();
			var maxX:int = mapData.length-1;
			var mapH:int = mapData[0].length;
			var randomY1:int = (Math.random()+mapH/2)%mapH;
			var randomY2:int = Math.random()*mapH;
			_tmpFindPathArr = pathFinder.findPath(new Point(0,randomY1), new Point(maxX, randomY2));
			if(null == _tmpFindPathArr){
				Message.show("(0,"+randomY1+") -> ("+maxX+","+randomY2+")没有找到合适路径");
				eventManager.dispatchToController(new ParamEvent(SceneEvent.PATH_SHOW_COMPLETE));
				return;
			}
			drawPathPoint(new Point(0,randomY1));
			drawPathPoint(new Point(maxX,randomY2));
			TimerManager.getInstance().removeItem(showPathTimerHandler);
			TimerManager.getInstance().addItem(50, showPathTimerHandler);
		}
		
		private function showPathTimerHandler():void
		{
			if(null == _tmpFindPathArr || 0 == _tmpFindPathArr.length){
				TimerManager.getInstance().removeItem(showPathTimerHandler);
				eventManager.dispatchToController(new ParamEvent(SceneEvent.PATH_SHOW_COMPLETE));
				return;
			}
			var p:Point = _tmpFindPathArr.shift();
			drawPathPoint(p);
		}
		/**
		 * 测试寻路并把路径画出来 
		 */		
		private function showFindPath():void
		{
			var mapData:Array = manager.getMapData();
			var maxX:int = mapData.length-1;
			var mapH:int = mapData[0].length;
			var randomY1:int = (Math.random()+mapH/2)%mapH;
			var randomY2:int = Math.random()*mapH;
			var path:Array = pathFinder.findPath(new Point(0,randomY1), new Point(maxX, randomY2));
			if(null == path){
				Message.show("(0,"+randomY1+") -> ("+maxX+","+randomY2+")没有找到合适路径");
				eventManager.dispatchToController(new ParamEvent(SceneEvent.PATH_SHOW_COMPLETE));
				return;
			}
			drawPathPoint(new Point(0,randomY1));
			drawPathPoint(new Point(maxX,randomY2));
			for (var i:int = 0; i < path.length; i++) 
			{
				var p:Point = path[i];
				drawPathPoint(p);
			}
		}
		
		/**
		 * 画出路径点（在路径层里） 
		 * @param p
		 */		
		private function drawPathPoint(p:Point):void
		{
			var pathItem:Shape = new Shape();
			pathItem.graphics.beginFill(0xff00ff,0.5);
			pathItem.graphics.drawRect(1,1,MapItem.WIDTH, MapItem.HEIGHT);
			pathItem.graphics.endFill();
			pathItem.x = p.x*MapItem.WIDTH;
			pathItem.y = p.y*MapItem.HEIGHT;
			pathLayer.addChild(pathItem);
		}
		
		protected function onRestartDemoHandler(event:ParamEvent):void
		{
			clearMaze();
			initMaze();
			
//			showFindPath();
			clearPath();
			showFindPathTimer();
		}
		
		/**
		 * [继承] 恢复资源
		 * 
		 */		
		public override function resume():void
		{
			super.resume();
			manager.addEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
		}
		
		/**
		 * [继承] 释放资源
		 * 
		 */		
		public override function dispose():void
		{
			super.dispose();
			manager.removeEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
		}
	}
}