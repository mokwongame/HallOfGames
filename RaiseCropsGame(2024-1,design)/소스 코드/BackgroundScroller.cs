using UnityEngine;

public class BackgroundScroller : MonoBehaviour
{
    public GameObject background1;
    public GameObject background2;
    public float scrollSpeed = 2.0f;
    private float backgroundWidth;

    void Start()
    {
        // 배경 이미지의 가로 길이를 계산
        backgroundWidth = background1.GetComponent<SpriteRenderer>().bounds.size.x;
    }

    void Update()
    {
        // 배경 이미지를 왼쪽으로 이동
        background1.transform.position += Vector3.left * scrollSpeed * Time.deltaTime;
        background2.transform.position += Vector3.left * scrollSpeed * Time.deltaTime;

        // 첫 번째 배경이 화면 밖으로 나가면 두 번째 배경의 뒤로 이동
        if (background1.transform.position.x <= -backgroundWidth)
        {
            background1.transform.position = new Vector3(background2.transform.position.x + backgroundWidth, background1.transform.position.y, background1.transform.position.z);
        }

        // 두 번째 배경이 화면 밖으로 나가면 첫 번째 배경의 뒤로 이동
        if (background2.transform.position.x <= -backgroundWidth)
        {
            background2.transform.position = new Vector3(background1.transform.position.x + backgroundWidth, background2.transform.position.y, background2.transform.position.z);
        }
    }
}
