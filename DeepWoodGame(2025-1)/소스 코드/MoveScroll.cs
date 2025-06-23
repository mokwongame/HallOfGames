using UnityEngine;

public class MoveScroll : MonoBehaviour
{
    float ppu = 16;
    [SerializeField] 
    int imgWidth = 256;
    float imgUnit;

    [SerializeField] 
    float m_moveSpeed;

    void Start()
    {
        imgUnit = imgWidth / ppu;
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = -m_moveSpeed * Time.deltaTime;

        for (int i = 0; i < transform.childCount; i++)
        {
            Transform child = transform.GetChild(i);

            child.Translate(xoff, 0.0f, 0.0f);
            float xpos = child.position.x;
            if (xpos <= -imgUnit * 1.2f)
            {
                child.Translate(imgUnit * transform.childCount, 0.0f, 0.0f);
            }
        }

        
    }
}
