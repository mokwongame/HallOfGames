using UnityEngine;

public class Background : MonoBehaviour
{
    public GameObject BackGround1;
    public GameObject BackGround2;

    public float scrollSpeed = 2f;
    private float backgroundWidth;

    void Start()
    {
        // 스프라이트 렌더러의 크기로 이미지 너비 가져오기
        backgroundWidth = BackGround1.GetComponent<SpriteRenderer>().bounds.size.x;

        // BackGround2를 BackGround1 오른쪽 바로 옆에 위치시키기
        BackGround2.transform.position = new Vector3(BackGround1.transform.position.x + backgroundWidth, BackGround1.transform.position.y, BackGround1.transform.position.z);
    }

    void Update()
    {
        // 두 배경 모두 왼쪽으로 이동
        BackGround1.transform.position += scrollSpeed * Time.deltaTime * Vector3.left;
        BackGround2.transform.position += scrollSpeed * Time.deltaTime * Vector3.left;

        // BackGround1이 완전히 화면 왼쪽 밖으로 나가면 BackGround2 오른쪽에 붙이기
        if (BackGround1.transform.position.x <= -backgroundWidth)
        {
            BackGround1.transform.position = new Vector3(BackGround2.transform.position.x + backgroundWidth, BackGround1.transform.position.y, BackGround1.transform.position.z);
        }

        // BackGround2가 완전히 화면 왼쪽 밖으로 나가면 BackGround1 오른쪽에 붙이기
        if (BackGround2.transform.position.x <= -backgroundWidth)
        {
            BackGround2.transform.position = new Vector3(BackGround1.transform.position.x + backgroundWidth, BackGround2.transform.position.y, BackGround2.transform.position.z);
        }
    }
}
