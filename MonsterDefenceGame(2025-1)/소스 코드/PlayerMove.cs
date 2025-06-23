using UnityEngine;

public class PlayerMove : MonoBehaviour
{
    public Transform bottomFloorPos;
    public Transform middleFloorPos;
    public Transform topFloorPos;

    public float moveCooldown = 0.5f; // 쿨타임 0.5초
    private float lastMoveTime = -999f;

    private enum FloorLevel { Bottom, Middle, Top }
    private FloorLevel currentFloor = FloorLevel.Middle;

    void Start()
    {
        float y = transform.position.y;
        if (Mathf.Abs(y - bottomFloorPos.position.y) < 0.5f)
            currentFloor = FloorLevel.Bottom;
        else if (Mathf.Abs(y - middleFloorPos.position.y) < 0.5f)
            currentFloor = FloorLevel.Middle;
        else
            currentFloor = FloorLevel.Top;
    }

    void Update()
    {
        if (Time.time - lastMoveTime < moveCooldown) return; // 쿨타임 제한

        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            if (MoveUp())
                lastMoveTime = Time.time;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            if (MoveDown())
                lastMoveTime = Time.time;
        }
    }

    bool MoveUp()
    {
        switch (currentFloor)
        {
            case FloorLevel.Bottom:
                TeleportTo(middleFloorPos);
                currentFloor = FloorLevel.Middle;
                return true;
            case FloorLevel.Middle:
                TeleportTo(topFloorPos);
                currentFloor = FloorLevel.Top;
                return true;
        }
        return false;
    }

    bool MoveDown()
    {
        switch (currentFloor)
        {
            case FloorLevel.Top:
                TeleportTo(middleFloorPos);
                currentFloor = FloorLevel.Middle;
                return true;
            case FloorLevel.Middle:
                TeleportTo(bottomFloorPos);
                currentFloor = FloorLevel.Bottom;
                return true;
        }
        return false;
    }

    void TeleportTo(Transform targetPos)
    {
        if (targetPos != null)
        {
            transform.position = new Vector3(transform.position.x, targetPos.position.y, transform.position.z);
            AudioManager.Instance?.PlayFloorMoveSFX(); 

        }
        else
        {
            Debug.LogWarning("층 위치가 연결되지 않았습니다!");
        }
    }
}
